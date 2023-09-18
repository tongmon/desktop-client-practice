#include "MessengerService.hpp"
#include "DBConnectionPool.hpp"
#include "NetworkDefinition.hpp"

#include <boost/algorithm/string.hpp>
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
    std::vector<std::string> parsed;
    boost::split(parsed, m_client_request, boost::is_any_of("|"));
    std::string id = parsed[0], pw = parsed[1];

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

    boost::json::object chat_room_obj;

    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
    {
        const soci::row &r = *it;
        std::string creator_id = r.get<std::string>(0),
                    session_id = r.get<std::string>(1),
                    id = creator_id + "_" + session_id,
                    chat_room_path = boost::dll::program_location().parent_path().string() + "/" + id;

        // session 정보 json 파일 읽기
        std::stringstream str_buf;
        std::ifstream session_info(chat_room_path + "/session_info.json");
        if (session_info.is_open())
            str_buf << session_info.rdbuf();
        session_info.close();

        boost::json::error_code ec;
        boost::json::value jval = boost::json::parse(str_buf.str(), ec);
        const boost::json::object &obj = jval.as_object();

        // session 정보 json에서 필요한 정보 파싱
        std::string session_name = boost::json::value_to<std::string>(obj.at("session_name"));
        std::vector<std::string> recent_chat_dates;
        boost::json::array content_array;

        boost::filesystem::path root(chat_room_path);
        boost::filesystem::directory_iterator path_it{root};

        // 가장 최근 날짜로부터 가까운 내용 json 찾음, 일단 3일치만
        while (path_it != boost::filesystem::directory_iterator{})
        {
            auto child_path = *path_it;
            if (!boost::filesystem::is_directory(child_path))
                continue;
        }

        for (const auto &date : recent_chat_dates)
        {
            std::vector<std::string> parsed;
            boost::split(parsed, date, boost::is_any_of("-"));
            std::string year = parsed[0], month = parsed[1], day = parsed[2];

            str_buf.clear();
            session_info.open(chat_room_path + "/" + year + "/" + day + ".json");
            if (session_info.is_open())
                str_buf << session_info.rdbuf();
            session_info.close();

            boost::json::object chat_content;
            chat_content["chat_date"] = date;
            chat_content["content"] = str_buf.str();
            content_array.push_back(chat_content);
        }

        boost::json::object chat_obj;
        chat_obj["session_name"] = session_name;
        chat_obj["session_img"] = "Base64 인코딩된 Image Binary form을 적어서 여기다 넣으셈";
        chat_obj["content"] = content_array;

        chat_room_obj[creator_id + "_" + session_id] = chat_obj;
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