#include "TCPClient.hpp"

namespace TCPClientLocalNamespace
{

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

/*
void TCPClient::OnRequestComplete(std::shared_ptr<Session> session)
{
    system::error_code ignored_ec;

    session->m_sock.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);

    std::unique_lock<std::mutex> lock(m_active_sessions_guard);

    auto it = m_active_sessions.find(session->m_id);
    if (it != m_active_sessions.end())
        m_active_sessions.erase(it);

    lock.unlock();

    system::error_code ec;

    if (session->m_ec == system::errc::success && session->m_was_cancelled)
        ec = asio::error::operation_aborted;
    else
        ec = session->m_ec;

    if (session->m_callback)
        session->m_callback(session->m_id, session->m_response, ec);
};
*/

void TCPClient::OnRequestEnd(std::shared_ptr<Session> session, std::function<void(std::shared_ptr<Session>)> end_callback)
{
    system::error_code ignored_ec;

    session->m_sock.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);

    std::unique_lock<std::mutex> lock(m_active_sessions_guard);

    auto it = m_active_sessions.find(session->m_id);
    if (it != m_active_sessions.end())
        m_active_sessions.erase(it);

    lock.unlock();

    system::error_code ec;

    if (session->m_ec == system::errc::success && session->m_was_cancelled)
        session->m_ec = asio::error::operation_aborted;

    if (end_callback)
        end_callback(session);
};

void TCPClient::AsyncConnect(const std::string &raw_ip_address,
                             unsigned short port_num,
                             unsigned int request_id,
                             std::function<void(std::shared_ptr<Session>)> on_finish_connection)
{
    std::shared_ptr<Session> session(new Session(m_ios,
                                                 raw_ip_address,
                                                 port_num,
                                                 "",
                                                 request_id));

    session->m_sock.open(session->m_ep.protocol());

    std::unique_lock<std::mutex> lock(m_active_sessions_guard);
    m_active_sessions[request_id] = session;
    lock.unlock();

    session->m_sock.async_connect(session->m_ep,
                                  [this, session, on_finish_connection](const system::error_code &ec) {
                                      if (ec != boost::system::errc::success)
                                      {
                                          session->m_ec = ec;
                                          OnRequestEnd(session, on_finish_connection);
                                          return;
                                      }

                                      std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);

                                      if (session->m_was_cancelled)
                                      {
                                          OnRequestEnd(session, on_finish_connection);
                                          return;
                                      }

                                      if (on_finish_connection)
                                          on_finish_connection(session);
                                  });
}

void TCPClient::AsyncWrite(unsigned int request_id,
                           const std::string &request,
                           std::function<void(std::shared_ptr<Session>)> on_finish_write)
{
    std::shared_ptr<Session> session;

    std::unique_lock<std::mutex> lock(m_active_sessions_guard);
    if (m_active_sessions.find(request_id) == m_active_sessions.end())
    {
        on_finish_write(nullptr);
        return;
    }
    m_active_sessions[request_id]->m_request = request;
    session = m_active_sessions[request_id];
    lock.unlock();

    asio::async_write(session->m_sock,
                      asio::buffer(session->m_request),
                      [this, session, on_finish_write](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                          if (ec != boost::system::errc::success)
                          {
                              session->m_ec = ec;
                              OnRequestEnd(session, on_finish_write);
                              return;
                          }

                          std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);

                          if (session->m_was_cancelled)
                          {
                              OnRequestEnd(session, on_finish_write);
                              return;
                          }

                          if (on_finish_write)
                              on_finish_write(session);
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

    asio::async_read_until(session->m_sock,
                           session->m_response_buf,
                           delim,
                           [this, session, on_finish_read_until](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                               if (ec != boost::system::errc::success)
                                   session->m_ec = ec;
                               else
                               {
                                   std::istream strm(&session->m_response_buf);
                                   std::getline(strm, session->m_response);
                               }

                               // OnRequestEnd(session, on_finish_read_until);
                           });
}

/*
void TCPClient::AsyncRequestAndGetData(const std::string &request,
                                       const std::string &raw_ip_address,
                                       unsigned short port_num,
                                       unsigned int request_id,
                                       Callback callback)
{
    std::shared_ptr<Session> session(new Session(m_ios,
                                                 raw_ip_address,
                                                 port_num,
                                                 request,
                                                 request_id,
                                                 callback));

    session->m_sock.open(session->m_ep.protocol());

    std::unique_lock<std::mutex> lock(m_active_sessions_guard);
    m_active_sessions[request_id] = session;
    lock.unlock();

    session->m_sock.async_connect(session->m_ep,
                                  [this, session](const system::error_code &ec) {
                                      if (ec != boost::system::errc::success)
                                      {
                                          session->m_ec = ec;
                                          OnRequestComplete(session);
                                          return;
                                      }

                                      std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);

                                      if (session->m_was_cancelled)
                                      {
                                          OnRequestComplete(session);
                                          return;
                                      }

                                      asio::async_write(session->m_sock,
                                                        asio::buffer(session->m_request),
                                                        [this, session](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                                            if (ec != boost::system::errc::success)
                                                            {
                                                                session->m_ec = ec;
                                                                OnRequestComplete(session);
                                                                return;
                                                            }

                                                            std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);

                                                            if (session->m_was_cancelled)
                                                            {
                                                                OnRequestComplete(session);
                                                                return;
                                                            }

                                                            asio::async_read_until(session->m_sock,
                                                                                   session->m_response_buf,
                                                                                   '\n',
                                                                                   [this, session](const boost::system::error_code &ec,
                                                                                                   std::size_t bytes_transferred) {
                                                                                       if (ec != boost::system::errc::success)
                                                                                       {
                                                                                           session->m_ec = ec;
                                                                                       }
                                                                                       else
                                                                                       {
                                                                                           std::istream strm(&session->m_response_buf);
                                                                                           std::getline(strm, session->m_response);
                                                                                       }

                                                                                       OnRequestComplete(session);
                                                                                   });
                                                        });
                                  });
}

*/

void TCPClient::CloseRequest(unsigned int request_id)
{
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

} // namespace TCPClientLocalNamespace