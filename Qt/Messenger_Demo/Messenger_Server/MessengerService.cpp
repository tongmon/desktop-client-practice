#include "MessengerService.hpp"
#include "DBConnectionPool.hpp"
#include "NetworkDefinition.hpp"

#include <iostream>
#include <regex>

MessengerService::MessengerService(std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    : Service(sock)
{
    m_sql = std::make_unique<soci::session>(DBConnectionPool::get());
}

// 서비스 종료 시 추가적으로 해제해야 할 것들 소멸자에 기입
MessengerService::~MessengerService()
{
}

void MessengerService::LoginHandling()
{
    std::smatch match;
    std::regex_search(m_client_request, match, std::regex(R"(\|)"));
    std::string id = match.prefix().str(), pw = match.suffix().str();

    std::cout << "ID: " << id << "  Password: " << pw << "\n";

    soci::indicator ind;
    std::string pw_from_db;
    *m_sql << "select password from user_tb where user_id = :id", soci::into(pw_from_db, ind), soci::use(id);

    m_request = {char(pw_from_db == pw ? 1 : 0)};

    TCPHeader header(LOGIN_CONNECTION_TYPE, m_request.size());
    m_request = header.GetHeaderBuffer() + m_request;

    boost::asio::async_write(*m_sock,
                             boost::asio::buffer(m_request),
                             [this](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                 if (ec != boost::system::errc::success)
                                 {
                                     // write에 이상이 있는 경우
                                 }

                                 delete this;
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
                                    delete this;
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
                                                                delete this;
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