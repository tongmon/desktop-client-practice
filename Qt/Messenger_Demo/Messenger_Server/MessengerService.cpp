#include "MessengerService.hpp"
#include "NetworkDefinition.hpp"
#include <regex>

MessengerService::MessengerService(std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    : Service(sock)
{
}

MessengerService::~MessengerService()
{
}

void MessengerService::EndService()
{
    delete this;
}

void MessengerService::LoginHandling()
{
    std::smatch match;
    std::regex_search(m_client_request, match, std::regex("|"));
    std::string id = match.prefix().str(), pw = match.suffix().str();

    // DB 검사해서 id, pw 매칭 결과에 따라 data에 채워넣고 클라로 보내셈
    std::string data = "success";

    TCPHeader header(LOGIN_CONNECTION_TYPE, data.size());
    data = header.GetHeaderBuffer() + data;

    boost::asio::async_write(*m_sock,
                             boost::asio::buffer(data),
                             [this](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                 if (ec != boost::system::errc::success)
                                 {
                                     // write에 이상이 있는 경우
                                 }

                                 EndService();
                             });
}

void MessengerService::MessageHandling()
{
}

void MessengerService::StartHandling()
{
    boost::asio::async_read(*m_sock,
                            m_client_request_buf.prepare(TCP_HEADER_SIZE),
                            [this](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                if (ec != boost::system::errc::success)
                                {
                                    // 서버 처리가 비정상인 경우
                                    EndService();
                                    return;
                                }

                                m_client_request_buf.commit(bytes_transferred);
                                std::istream strm(&m_client_request_buf);
                                std::getline(strm, m_client_request);

                                TCPHeader header(m_client_request);
                                auto connection_type = header.GetConnectionType();
                                auto data_size = header.GetDataSize();

                                boost::asio::async_read(*m_sock,
                                                        m_client_request_buf.prepare(data_size),
                                                        [this, &connection_type](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                                            if (ec != boost::system::errc::success)
                                                            {
                                                                // 서버 처리가 비정상인 경우
                                                                EndService();
                                                                return;
                                                            }

                                                            m_client_request_buf.commit(bytes_transferred);
                                                            std::istream strm(&m_client_request_buf);
                                                            std::getline(strm, m_client_request);

                                                            switch (connection_type)
                                                            {
                                                            case LOGIN_CONNECTION_TYPE:
                                                                LoginHandling();
                                                                break;
                                                            case TEXTCHAT_CONNECTION_TYPE:
                                                                MessageHandling();
                                                                break;
                                                            default:
                                                                break;
                                                            }
                                                        });
                            });
}