#include "TCPServer.hpp"

namespace TCPServerLocalNamespace
{
void Service::OnRequestReceived(const system::error_code &ec, std::size_t bytes_transferred)
{
    if (ec != system::errc::success)
    {
        // std::cout << "Error occured! Error code = "
        //           << ec.value()
        //           << ". Message: " << ec.message();

        OnFinish();
        return;
    }

    // Process the request.
    m_response = ProcessRequest(m_request);

    // Initiate asynchronous write operation.
    asio::async_write(*m_sock.get(),
                      asio::buffer(m_response),
                      [this](const system::error_code &ec, std::size_t bytes_transferred) {
                          OnResponseSent(ec, bytes_transferred);
                      });
}

void Service::OnResponseSent(const system::error_code &ec, std::size_t bytes_transferred)
{
    if (ec != system::errc::success)
    {
        // std::cout << "Error occured! Error code = "
        //           << ec.value()
        //           << ". Message: " << ec.message();
    }

    OnFinish();
}

void Service::OnFinish()
{
    delete this;
}

// 클라이언트에서 전달된 request를 활용하는 실제 로직은 여기에 위치한다.
std::string Service::ProcessRequest(asio::streambuf &request)
{
    std::string req((std::istreambuf_iterator<char>(&request)), std::istreambuf_iterator<char>());

    std::cout << req << "\n";

    // 실제 로직
    std::string response = "success\n";
    return response;
}

Service::Service(std::shared_ptr<asio::ip::tcp::socket> sock)
    : m_sock(sock)
{
}

void Service::StartHandling()
{
    asio::async_read_until(*m_sock.get(),
                           m_request,
                           '\n',
                           [this](
                               const system::error_code &ec, std::size_t bytes_transferred) {
                               OnRequestReceived(ec, bytes_transferred);
                           });
}

void Acceptor::InitAccept()
{
    std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(m_ios));

    m_acceptor.async_accept(*sock.get(),
                            [this, sock](const system::error_code &error) {
                                OnAccept(error, sock);
                            });
}

void Acceptor::OnAccept(const system::error_code &ec,
                        std::shared_ptr<asio::ip::tcp::socket> sock)
{
    if (ec == system::errc::success)
        (new Service(sock))->StartHandling();
    else
    {
        // std::cout << "Error occured! Error code = "
        //           << ec.value()
        //           << ". Message: " << ec.message();
    }

    // Init next async accept operation if
    // acceptor has not been stopped yet.
    if (!m_isStopped.load())
        InitAccept();
    else
        m_acceptor.close();
}

Acceptor::Acceptor(asio::io_service &ios, unsigned short port_num)
    : m_ios(ios),
      m_acceptor(m_ios, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port_num)),
      m_isStopped(false)
{
}

void Acceptor::Start()
{
    m_acceptor.listen();
    InitAccept();
}

void Acceptor::Stop()
{
    m_isStopped.store(true);
}

TCPServer::TCPServer()
{
    m_work.reset(new asio::io_service::work(m_ios));
}

void TCPServer::Start(unsigned short port_num, unsigned int thread_pool_size)
{
    assert(thread_pool_size > 0);

    // Create and start Acceptor.
    acc.reset(new Acceptor(m_ios, port_num));
    acc->Start();

    // Create specified number of threads and
    // add them to the pool.
    for (int i = 0; i < thread_pool_size; i++)
    {
        std::unique_ptr<std::thread> th(
            new std::thread([this]() {
                m_ios.run();
            }));

        m_thread_pool.push_back(std::move(th));
    }
}

void TCPServer::Stop()
{
    acc->Stop();
    m_ios.stop();

    for (auto &th : m_thread_pool)
        th->join();
}
} // namespace TCPServerLocalNamespace