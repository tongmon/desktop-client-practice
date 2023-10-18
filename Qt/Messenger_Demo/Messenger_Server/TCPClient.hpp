#ifndef HEADER__FILE__TCPCLIENT
#define HEADER__FILE__TCPCLIENT

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

// Boost.Asio Windows 7 이상을 타겟으로 설정
// #define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <boost/asio.hpp>

class Session
{
    friend class TCPClient;

    boost::asio::ip::tcp::socket m_sock;
    boost::asio::ip::tcp::endpoint m_ep;
    std::string m_request;

    boost::asio::streambuf m_response_buf;
    std::string m_response;

    boost::system::error_code m_ec;

    unsigned int m_id;

    bool m_was_cancelled;
    std::mutex m_cancel_guard;

  public:
    Session(boost::asio::io_service &ios,
            const std::string &raw_ip_address,
            unsigned short port_num,
            const std::string &request,
            unsigned int id)
        : m_sock(ios),
          m_ep(boost::asio::ip::address::from_string(raw_ip_address), port_num),
          m_request(request),
          m_id(id),
          m_was_cancelled(false)
    {
    }

    bool IsValid()
    {
        return m_ec == boost::system::errc::success;
    }

    std::string GetResponse()
    {
        return m_response;
    }

    unsigned int GetID()
    {
        return m_id;
    }
};

class TCPClient
{
    boost::asio::io_service m_ios;
    std::map<unsigned int, std::shared_ptr<Session>> m_active_sessions;
    std::mutex m_active_sessions_guard;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::list<std::unique_ptr<std::thread>> m_threads;

  public:
    TCPClient(unsigned char num_of_threads);
    ~TCPClient();

    bool AsyncConnect(const std::string &raw_ip_address,
                      unsigned short port_num,
                      unsigned int request_id,
                      std::function<void(std::shared_ptr<Session>)> on_success_connection = {});

    void AsyncWrite(unsigned int request_id,
                    const std::string &request,
                    std::function<void(std::shared_ptr<Session>)> on_finish_write = {});

    void AsyncRead(unsigned int request_id,
                   size_t buffer_size,
                   std::function<void(std::shared_ptr<Session>)> on_finish_read);

    void AsyncReadUntil(unsigned int request_id,
                        std::function<void(std::shared_ptr<Session>)> on_finish_read_until,
                        const std::string &delim = "\n");

    std::shared_ptr<Session> CloseRequest(unsigned int request_id);

    void CancelRequest(unsigned int request_id);

    void Close();

    unsigned int MakeRequestID();
};

#endif /* HEADER__FILE__TCPCLIENT */
