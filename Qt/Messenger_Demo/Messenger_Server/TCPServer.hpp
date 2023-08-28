#ifndef HEADER__FILE__TCPSERVER
#define HEADER__FILE__TCPSERVER

#include <memory>
#include <string>
#include <thread>

#include <iostream>

// Boost.Asio Windows 7 이상을 타겟으로 설정
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <boost/asio.hpp>

// 이 녀석은 무조건 동적할당으로 수행한다.
// class Service
// {
//     std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
//     std::string m_response;
//     boost::asio::streambuf m_request;
//
//     void OnRequestReceived(const boost::system::error_code &ec, std::size_t bytes_transferred);
//
//     void OnResponseSent(const boost::system::error_code &ec, std::size_t bytes_transferred);
//
//     // 동적으로 할당되기에 자기 자신을 지우는 것을 가정한다.
//     void OnFinish();
//
//     // 클라이언트에서 전달된 request를 활용하는 실제 로직은 여기에 위치한다.
//     std::string ProcessRequest(boost::asio::streambuf &request);
//
//   public:
//     Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock);
//
//     void StartHandling();
// };
//
// class Acceptor
// {
//     boost::asio::io_service &m_ios;
//     boost::asio::ip::tcp::acceptor m_acceptor;
//     std::atomic<bool> m_is_stopped;
//
//     void InitAccept();
//
//     void OnAccept(const boost::system::error_code &ec, std::shared_ptr<boost::asio::ip::tcp::socket> sock);
//
//   public:
//     Acceptor(boost::asio::io_service &ios, unsigned short port_num);
//
//     // Start accepting incoming connection requests.
//     void Start();
//
//     // Stop accepting incoming connection requests.
//     void Stop();
// };
//
// class TCPServer
// {
//     boost::asio::io_service m_ios;
//     std::unique_ptr<boost::asio::io_service::work> m_work;
//     std::unique_ptr<Acceptor> acc;
//     std::vector<std::unique_ptr<std::thread>> m_thread_pool;
//
//   public:
//     TCPServer();
//
//     // Start the server.
//     void Start(unsigned short port_num, unsigned int thread_pool_size);
//
//     // Stop the server.
//     void Stop();
// };

template <typename Service>
class TCPServer
{
    boost::asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;

    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
    std::atomic<bool> m_is_stopped;

    void InitAcceptor(const unsigned short &port_num)
    {
        m_acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(m_ios,
                                                                      boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(),
                                                                                                     port_num));
        m_is_stopped.store(false);
        m_acceptor->listen();

        std::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_ios));
        m_acceptor->async_accept(*sock.get(),
                                 [this, sock](const boost::system::error_code &error) {
                                     while (!m_is_stopped.load())
                                     {
                                         if (error == boost::system::errc::success)
                                             (new Service(sock))->StartHandling();
                                         else
                                         {
                                             // Accept 실패시 동작 정의
                                             // break;
                                         }
                                     }

                                     m_acceptor->close();
                                 });
    }

  public:
    TCPServer(const unsigned short &port_num, unsigned int thread_pool_size)
    {
        m_acceptor.reset(nullptr);
        m_work.reset(new boost::asio::io_service::work(m_ios));

        // acceptor 연결 설정
        InitAcceptor(port_num);

        // 쓰레드 풀 생성
        for (int i = 0; i < thread_pool_size; i++)
        {
            std::unique_ptr<std::thread> th(
                new std::thread([this]() {
                    m_ios.run();
                }));
            m_thread_pool.push_back(std::move(th));
        }
    }

    ~TCPServer()
    {
        m_is_stopped.store(true);
        m_ios.stop();

        for (auto &th : m_thread_pool)
            th->join();
    }
};

#endif /* HEADER__FILE__TCPSERVER */
