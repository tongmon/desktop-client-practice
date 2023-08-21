#ifndef HEADER__FILE__TCPSERVER
#define HEADER__FILE__TCPSERVER

#include <memory>
#include <string>
#include <thread>

#include <iostream>

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

    void OnRequestReceived(const system::error_code &ec, std::size_t bytes_transferred);

    void OnResponseSent(const system::error_code &ec, std::size_t bytes_transferred);

    // Here we perform the cleanup.
    void OnFinish();

    // 클라이언트에서 전달된 request를 활용하는 실제 로직은 여기에 위치한다.
    std::string ProcessRequest(asio::streambuf &request);

  public:
    Service(std::shared_ptr<asio::ip::tcp::socket> sock);

    void StartHandling();
};

class Acceptor
{
    asio::io_service &m_ios;
    asio::ip::tcp::acceptor m_acceptor;
    std::atomic<bool> m_isStopped;

    void InitAccept();

    void OnAccept(const system::error_code &ec, std::shared_ptr<asio::ip::tcp::socket> sock);

  public:
    Acceptor(asio::io_service &ios, unsigned short port_num);

    // Start accepting incoming connection requests.
    void Start();

    // Stop accepting incoming connection requests.
    void Stop();
};

class TCPServer
{
    asio::io_service m_ios;
    std::unique_ptr<asio::io_service::work> m_work;
    std::unique_ptr<Acceptor> acc;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;

  public:
    TCPServer();

    // Start the server.
    void Start(unsigned short port_num, unsigned int thread_pool_size);

    // Stop the server.
    void Stop();
};
} // namespace TCPServerLocalNamespace

using TCPServerLocalNamespace::TCPServer;

#endif /* HEADER__FILE__TCPSERVER */
