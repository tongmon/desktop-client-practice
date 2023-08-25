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
class Service
{
    std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    std::string m_response;
    boost::asio::streambuf m_request;

    void OnRequestReceived(const boost::system::error_code &ec, std::size_t bytes_transferred);

    void OnResponseSent(const boost::system::error_code &ec, std::size_t bytes_transferred);

    // 동적으로 할당되기에 자기 자신을 지우는 것을 가정한다.
    void OnFinish();

    // 클라이언트에서 전달된 request를 활용하는 실제 로직은 여기에 위치한다.
    std::string ProcessRequest(boost::asio::streambuf &request);

  public:
    Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock);

    void StartHandling();
};

class Acceptor
{
    boost::asio::io_service &m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::atomic<bool> m_is_stopped;

    void InitAccept();

    void OnAccept(const boost::system::error_code &ec, std::shared_ptr<boost::asio::ip::tcp::socket> sock);

  public:
    Acceptor(boost::asio::io_service &ios, unsigned short port_num);

    // Start accepting incoming connection requests.
    void Start();

    // Stop accepting incoming connection requests.
    void Stop();
};

class TCPServer
{
    boost::asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::unique_ptr<Acceptor> acc;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;

  public:
    TCPServer();

    // Start the server.
    void Start(unsigned short port_num, unsigned int thread_pool_size);

    // Stop the server.
    void Stop();
};

#endif /* HEADER__FILE__TCPSERVER */
