#ifndef HEADER__FILE__SERVICE
#define HEADER__FILE__SERVICE

// Boost.Asio Windows 7 이상을 타겟으로 설정
// #define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <boost/asio.hpp>
#include <mutex>
#include <unordered_map>

class Service
{
  protected:
    std::unordered_map<std::string, std::shared_ptr<boost::asio::ip::tcp::socket>> &m_sock_map;
    std::mutex &m_sock_map_mut;
    std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;

  public:
    Service(std::unordered_map<std::string, std::shared_ptr<boost::asio::ip::tcp::socket>> &sock_map,
            std::mutex &sock_map_mut,
            std::shared_ptr<boost::asio::ip::tcp::socket> sock)
        : m_sock_map{sock_map}, m_sock_map_mut{sock_map_mut}, m_sock{sock}
    {
    }
    virtual void StartHandling() = 0;
};

#endif /* HEADER__FILE__SERVICE */
