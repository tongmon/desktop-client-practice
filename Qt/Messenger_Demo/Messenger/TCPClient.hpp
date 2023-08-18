#ifndef HEADER__FILE__TCPCLIENT
#define HEADER__FILE__TCPCLIENT

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

// Boost.Asio Windows 7 이상을 타겟으로 설정
#define _WIN32_WINNT 0x0601

#include <boost/asio.hpp>

namespace TCPClientLocalNamespace
{
using namespace boost;

using Callback = std::function<void(unsigned int request_id, const std::string &response, const system::error_code &ec)>;

struct Session
{
    Session(asio::io_service &ios,
            const std::string &raw_ip_address,
            unsigned short port_num,
            const std::string &request,
            unsigned int id,
            Callback callback)
        : m_sock(ios),
          m_ep(asio::ip::address::from_string(raw_ip_address), port_num),
          m_request(request),
          m_id(id),
          m_callback(callback),
          m_was_cancelled(false)
    {
    }

    asio::ip::tcp::socket m_sock;
    asio::ip::tcp::endpoint m_ep;
    std::string m_request;

    asio::streambuf m_response_buf;
    std::string m_response;

    system::error_code m_ec;

    unsigned int m_id;

    Callback m_callback;

    bool m_was_cancelled;
    std::mutex m_cancel_guard;
};

class TCPClient
{
    asio::io_service m_ios;
    std::map<unsigned int, std::shared_ptr<Session>> m_active_sessions;
    std::mutex m_active_sessions_guard;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::list<std::unique_ptr<std::thread>> m_threads;

    void OnRequestComplete(std::shared_ptr<Session> session);

  public:
    TCPClient(unsigned char num_of_threads);
    ~TCPClient();

    void AsyncRequestAndGetData(const std::string &request,
                                const std::string &raw_ip_address,
                                unsigned short port_num,
                                unsigned int request_id,
                                Callback callback);

    void CancelRequest(unsigned int request_id);

    void Close();
};
} // namespace TCPClientLocalNamespace

using TCPClientLocalNamespace::TCPClient;

#endif /* HEADER__FILE__TCPCLIENT */
