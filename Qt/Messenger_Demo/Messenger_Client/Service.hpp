#ifndef HEADER__FILE__SERVICE
#define HEADER__FILE__SERVICE

// Boost.Asio Windows 7 이상을 타겟으로 설정
// #define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <boost/asio.hpp>

class TCPClient;

class Service
{
  protected:
    std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    std::shared_ptr<TCPClient> m_peer;

  public:
    Service(std::shared_ptr<TCPClient> peer, std::shared_ptr<boost::asio::ip::tcp::socket> sock)
        : m_peer{peer}, m_sock{sock}
    {
    }
    virtual void StartHandling() = 0;
};

#endif /* HEADER__FILE__SERVICE */
