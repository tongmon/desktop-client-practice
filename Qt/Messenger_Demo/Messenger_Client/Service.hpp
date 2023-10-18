#ifndef HEADER__FILE__SERVICE
#define HEADER__FILE__SERVICE

// Boost.Asio Windows 7 이상을 타겟으로 설정
// #define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <boost/asio.hpp>
#include <memory>
#include <string>

class WinQuickWindow;

class Service
{
    std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    WinQuickWindow &m_window;

    std::string m_request;
    std::string m_server_request;
    boost::asio::streambuf m_server_request_buf;

  public:
    Service(WinQuickWindow &window, std::shared_ptr<boost::asio::ip::tcp::socket> sock);

    void StartHandling();
};

#endif /* HEADER__FILE__SERVICE */
