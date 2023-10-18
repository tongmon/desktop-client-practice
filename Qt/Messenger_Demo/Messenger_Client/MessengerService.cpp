#include "MessengerService.hpp"
#include "DBConnectionPool.hpp"
#include "NetworkDefinition.hpp"
#include "TCPClient.hpp"
#include "Utility.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/json.hpp>
#include <chrono>
#include <ctime>
#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

MessengerService::MessengerService(std::shared_ptr<TCPClient> peer, std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    : Service(peer, sock)
{
    m_sql = std::make_unique<soci::session>(DBConnectionPool::Get());
}

// 서비스 종료 시 추가적으로 해제해야 할 것들 소멸자에 기입
MessengerService::~MessengerService()
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

                                m_server_request_buf.commit(bytes_transferred);
                                std::istream strm(&m_server_request_buf);
                                std::getline(strm, m_server_request);

                                TCPHeader header(m_server_request);
                                auto connection_type = header.GetConnectionType();
                                auto data_size = header.GetDataSize();

                                boost::asio::async_read(*m_sock,
                                                        m_server_request_buf.prepare(data_size),
                                                        [this, &connection_type](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                                            if (ec != boost::system::errc::success)
                                                            {
                                                                // 서버 처리가 비정상인 경우
                                                                delete this;
                                                                return;
                                                            }

                                                            m_server_request_buf.commit(bytes_transferred);
                                                            std::istream strm(&m_server_request_buf);
                                                            std::getline(strm, m_server_request);

                                                            switch (connection_type)
                                                            {
                                                            case LOGIN_CONNECTION_TYPE:
                                                                break;
                                                            case TEXTCHAT_CONNECTION_TYPE:
                                                                break;
                                                            case CHATROOMLIST_INITIAL_TYPE:
                                                                break;
                                                            default:
                                                                break;
                                                            }
                                                        });
                            });
}