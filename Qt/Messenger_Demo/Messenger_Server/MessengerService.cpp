#include "MessengerService.hpp"
#include "DBConnectionPool.hpp"
#include "NetworkDefinition.hpp"

#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/json.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

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

    if (ind == soci::i_ok)
        m_request = {char(pw_from_db == pw ? 1 : 0)};
    else
        m_request = {0};

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

void MessengerService::ChatRoomListInitHandling()
{
    soci::rowset<soci::row> rs = (m_sql->prepare << "select creator_id, session_id from session_user_relation_tb where user_id=:id",
                                  soci::use(m_client_request, "id"));

    soci::indicator ind;

    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
    {
        const soci::row &r = *it;
        std::string id = r.get<std::string>(0) + "_" + r.get<std::string>(1),
                    chat_room_path = boost::dll::program_location().parent_path().string() + "/" + id + "/";

        // session 정보 json 파일 읽기
        std::stringstream str_buf;
        std::ifstream session_info(chat_room_path + "session_info.json");
        if (session_info.is_open())
            str_buf << session_info.rdbuf();
        session_info.close();

        boost::json::error_code ec;
        boost::json::value jval = boost::json::parse(str_buf.str(), ec);
        const boost::json::object &obj = jval.as_object();

        // session 정보 json에서 session_img_path, session_name 파싱
        std::string session_img_path = boost::json::value_to<std::string>(obj.at("session_img_path")),
                    session_name = boost::json::value_to<std::string>(obj.at("session_name"));

        // 폴더 내부에 있는 파일 이름역순으로 3개 탐색하는 코드 짜야됨
        // boost::filesystem
    }
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
                                                            case CHATROOMLIST_INITIAL_TYPE:
                                                                ChatRoomListInitHandling();
                                                                break;
                                                            default:
                                                                break;
                                                            }
                                                        });
                            });
}