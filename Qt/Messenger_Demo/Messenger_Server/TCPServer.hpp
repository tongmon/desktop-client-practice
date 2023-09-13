#ifndef HEADER__FILE__TCPSERVER
#define HEADER__FILE__TCPSERVER

#include <memory>
#include <string>
#include <thread>

// Boost.Asio Windows 7 이상을 타겟으로 설정
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <boost/asio.hpp>

template <typename Service>
class TCPServer
{
    boost::asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;

    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
    std::atomic<bool> m_is_stopped;

    void StartAcceptor()
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_ios));
        m_acceptor->async_accept(*sock,
                                 [this, sock](const boost::system::error_code &error) {
                                     OnAccept(sock, error);
                                 });
    }

    void OnAccept(std::shared_ptr<boost::asio::ip::tcp::socket> sock, const boost::system::error_code &ec)
    {
        if (ec == boost::system::errc::success)
            (new Service(sock))->StartHandling();
        else
        {
            // Accept 실패시 동작 정의
        }

        // 서버가 멈추지 않았다면 async_accept 계속 수행
        m_is_stopped.load() ? m_acceptor->close() : StartAcceptor();
    }

  public:
    TCPServer(const unsigned short &port_num, unsigned int thread_pool_size)
    {
        m_acceptor.reset(nullptr);
        m_work.reset(new boost::asio::io_service::work(m_ios));

        m_acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(m_ios,
                                                                      boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(),
                                                                                                     port_num));
        m_is_stopped.store(false);
        m_acceptor->listen();

        StartAcceptor();

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
