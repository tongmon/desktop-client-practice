#ifndef HEADER__FILE__TCPSERVER
#define HEADER__FILE__TCPSERVER

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

// Boost.Asio Windows 7 이상을 타겟으로 설정
// #define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <boost/asio.hpp>

class WinQuickWindow;

class TCPServer
{
    boost::asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;

    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
    std::atomic<bool> m_is_stopped;

    WinQuickWindow &m_window;

    void StartAcceptor();

    void OnAccept(std::shared_ptr<boost::asio::ip::tcp::socket> sock, const boost::system::error_code &ec);

  public:
    TCPServer(WinQuickWindow &window, const unsigned short &port_num, unsigned int thread_pool_size);

    ~TCPServer();

    std::string GetIPAddress();

    unsigned short GetPortNumber();
};

#endif /* HEADER__FILE__TCPSERVER */
