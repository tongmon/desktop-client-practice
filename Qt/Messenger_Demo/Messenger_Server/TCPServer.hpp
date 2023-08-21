#ifndef HEADER__FILE__TCPSERVER
#define HEADER__FILE__TCPSERVER

#include <memory>
#include <string>
#include <thread>

// Boost.Asio Windows 7 이상을 타겟으로 설정
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <boost/asio.hpp>

namespace TCPServerLocalNamespace
{
using namespace boost;

// 이 녀석은 무조건 동적할당으로 수행한다.
class Service
{
    std::shared_ptr<asio::ip::tcp::socket> m_sock;
    std::string m_response;
    asio::streambuf m_request;

    void OnRequestReceived(const system::error_code &ec,
                           std::size_t bytes_transferred)
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

    void OnResponseSent(const system::error_code &ec, std::size_t bytes_transferred)
    {
        if (ec != system::errc::success)
        {
            // std::cout << "Error occured! Error code = "
            //           << ec.value()
            //           << ". Message: " << ec.message();
        }

        OnFinish();
    }

    // Here we perform the cleanup.
    void OnFinish()
    {
        delete this;
    }

    // 클라이언트에서 전달된 request를 활용하는 실제 로직은 여기에 위치한다.
    std::string ProcessRequest(asio::streambuf &request)
    {
        // 실제 로직
        std::string response = "Response\n";
        return response;
    }

  public:
    Service(std::shared_ptr<asio::ip::tcp::socket> sock)
        : m_sock(sock)
    {
    }

    void StartHandling()
    {
        asio::async_read_until(*m_sock.get(),
                               m_request,
                               '\n',
                               [this](
                                   const system::error_code &ec, std::size_t bytes_transferred) {
                                   OnRequestReceived(ec, bytes_transferred);
                               });
    }
};

class Acceptor
{
    asio::io_service &m_ios;
    asio::ip::tcp::acceptor m_acceptor;
    std::atomic<bool> m_isStopped;

    void InitAccept()
    {
        std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(m_ios));

        m_acceptor.async_accept(*sock.get(),
                                [this, sock](const system::error_code &error) {
                                    OnAccept(error, sock);
                                });
    }

    void OnAccept(const system::error_code &ec,
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

  public:
    Acceptor(asio::io_service &ios, unsigned short port_num)
        : m_ios(ios),
          m_acceptor(m_ios,
                     asio::ip::tcp::endpoint(
                         asio::ip::address_v4::any(),
                         port_num)),
          m_isStopped(false)
    {
    }

    // Start accepting incoming connection requests.
    void Start()
    {
        m_acceptor.listen();
        InitAccept();
    }

    // Stop accepting incoming connection requests.
    void Stop()
    {
        m_isStopped.store(true);
    }
};

class TCPServer
{
    asio::io_service m_ios;
    std::unique_ptr<asio::io_service::work> m_work;
    std::unique_ptr<Acceptor> acc;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;

  public:
    TCPServer()
    {
        m_work.reset(new asio::io_service::work(m_ios));
    }

    // Start the server.
    void Start(unsigned short port_num, unsigned int thread_pool_size)
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

    // Stop the server.
    void Stop()
    {
        acc->Stop();
        m_ios.stop();

        for (auto &th : m_thread_pool)
            th->join();
    }
};
} // namespace TCPServerLocalNamespace

using TCPServerLocalNamespace::TCPServer;

#endif /* HEADER__FILE__TCPSERVER */
