#include "TCPClient.hpp"

TCPClient::TCPClient(unsigned char num_of_threads)
{
    m_work.reset(new boost::asio::io_service::work(m_ios));

    for (unsigned char i = 0; i < num_of_threads; i++)
    {
        std::unique_ptr<std::thread> th(
            new std::thread([this]() {
                m_ios.run();
            }));

        m_threads.push_back(std::move(th));
    }
}

TCPClient::~TCPClient()
{
}

bool TCPClient::AsyncConnect(const std::string &raw_ip_address,
                             unsigned short port_num,
                             unsigned int request_id,
                             std::function<void(std::shared_ptr<Session>)> on_finish_connection)
{
    std::shared_ptr<Session> session;
    std::unique_lock<std::mutex> lock(m_active_sessions_guard);

    if (m_active_sessions.find(request_id) == m_active_sessions.end())
        session = std::make_shared<Session>(m_ios, raw_ip_address, port_num, "", request_id);
    else
        return false;
    m_active_sessions[request_id] = session;

    lock.unlock();

    session->m_sock.open(session->m_ep.protocol());

    session->m_sock.async_connect(session->m_ep,
                                  [this, session, on_finish_connection](const boost::system::error_code &ec) {
                                      if (ec != boost::system::errc::success)
                                      {
                                          session->m_ec = ec;
                                          if (on_finish_connection)
                                              on_finish_connection(CloseRequest(session->m_id));
                                          return;
                                      }

                                      std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);

                                      if (session->m_was_cancelled)
                                      {
                                          if (on_finish_connection)
                                              on_finish_connection(CloseRequest(session->m_id));
                                          return;
                                      }

                                      if (on_finish_connection)
                                          on_finish_connection(session);
                                  });

    return true;
}

void TCPClient::AsyncWrite(unsigned int request_id,
                           const std::string &request,
                           std::function<void(std::shared_ptr<Session>)> on_finish_write)
{
    std::shared_ptr<Session> session;

    std::unique_lock<std::mutex> lock(m_active_sessions_guard);
    if (m_active_sessions.find(request_id) == m_active_sessions.end())
    {
        if (on_finish_write)
            on_finish_write(nullptr);
        return;
    }
    m_active_sessions[request_id]->m_request = request;
    session = m_active_sessions[request_id];
    lock.unlock();

    boost::asio::async_write(session->m_sock,
                             boost::asio::buffer(session->m_request),
                             [this, session, on_finish_write](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                 if (ec != boost::system::errc::success)
                                 {
                                     session->m_ec = ec;
                                     if (on_finish_write)
                                         on_finish_write(CloseRequest(session->m_id));
                                     return;
                                 }

                                 std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);

                                 if (session->m_was_cancelled)
                                 {
                                     if (on_finish_write)
                                         on_finish_write(CloseRequest(session->m_id));
                                     return;
                                 }

                                 if (on_finish_write)
                                     on_finish_write(session);
                             });
}

void TCPClient::AsyncRead(unsigned int request_id, size_t buffer_size, std::function<void(std::shared_ptr<Session>)> on_finish_read)
{
    std::shared_ptr<Session> session;

    std::unique_lock<std::mutex> lock(m_active_sessions_guard);
    if (m_active_sessions.find(request_id) == m_active_sessions.end())
    {
        on_finish_read(nullptr);
        return;
    }
    session = m_active_sessions[request_id];
    lock.unlock();

    boost::asio::async_read(session->m_sock,
                            session->m_response_buf.prepare(buffer_size),
                            [this, session, on_finish_read](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                if (ec != boost::system::errc::success)
                                {
                                    session->m_ec = ec;
                                    if (on_finish_read)
                                        on_finish_read(CloseRequest(session->m_id));
                                    return;
                                }

                                session->m_response_buf.commit(bytes_transferred);
                                std::istream strm(&session->m_response_buf);
                                std::getline(strm, session->m_response);

                                if (on_finish_read)
                                    on_finish_read(session);
                            });
}

void TCPClient::AsyncReadUntil(unsigned int request_id,
                               std::function<void(std::shared_ptr<Session>)> on_finish_read_until, const std::string &delim)
{
    std::shared_ptr<Session> session;

    std::unique_lock<std::mutex> lock(m_active_sessions_guard);
    if (m_active_sessions.find(request_id) == m_active_sessions.end())
    {
        on_finish_read_until(nullptr);
        return;
    }
    session = m_active_sessions[request_id];
    lock.unlock();

    boost::asio::async_read_until(session->m_sock,
                                  session->m_response_buf,
                                  delim,
                                  [this, session, on_finish_read_until](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                      if (ec != boost::system::errc::success)
                                      {
                                          session->m_ec = ec;
                                          if (on_finish_read_until)
                                              on_finish_read_until(CloseRequest(session->m_id));
                                          return;
                                      }

                                      std::istream strm(&session->m_response_buf);
                                      std::getline(strm, session->m_response);

                                      if (on_finish_read_until)
                                          on_finish_read_until(session);
                                  });
}

std::shared_ptr<Session> TCPClient::CloseRequest(unsigned int request_id)
{
    std::shared_ptr<Session> session;
    std::unique_lock<std::mutex> lock(m_active_sessions_guard);

    auto it = m_active_sessions.find(request_id);
    if (it != m_active_sessions.end())
    {
        session = it->second;
        m_active_sessions.erase(it);
    }
    else
        return nullptr;

    lock.unlock();

    boost::system::error_code ignored_ec;

    session->m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

    if (session->m_ec == boost::system::errc::success && session->m_was_cancelled)
        session->m_ec = boost::asio::error::operation_aborted;

    return session;
}

void TCPClient::CancelRequest(unsigned int request_id)
{
    std::unique_lock<std::mutex> lock(m_active_sessions_guard);

    auto it = m_active_sessions.find(request_id);
    if (it != m_active_sessions.end())
    {
        std::unique_lock<std::mutex> cancel_lock(it->second->m_cancel_guard);

        it->second->m_was_cancelled = true;
        it->second->m_sock.cancel();
    }
}

void TCPClient::Close()
{
    m_work.reset(NULL);

    for (auto &thread : m_threads)
    {
        thread->join();
    }
}