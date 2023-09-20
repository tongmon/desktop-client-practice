#include "MessengerService.hpp"
#include "DBConnectionPool.hpp"
#include "NetworkDefinition.hpp"
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
#include <stb_image.h>
#include <stb_image_write.h>

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

    boost::json::array chat_room_array;

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
        boost::json::value session_json = boost::json::parse(str_buf.str(), ec);
        const boost::json::object &obj = session_json.as_object();

        // session 정보 json에서 필요한 정보 파싱
        std::string session_name = boost::json::value_to<std::string>(obj.at("session_name"));
        boost::json::array content_array;

        boost::filesystem::path root(chat_room_path);
        boost::filesystem::directory_iterator path_it{root};

        // 현재 날짜
        auto cur_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct tm cur_time;
        gmtime_s(&cur_time, &cur_time_t); // 윈도우에서만 사용함, thread-safe

        // 가장 최근 날짜로부터 가까운 내용 json 찾음, 일단 3일치만
        for (int year = cur_time.tm_year; year >= 1900; year--)
        {
            std::string path_year = chat_room_path + "/" + std::to_string(year);
            if (!boost::filesystem::exists(path_year))
                continue;

            // 해당 년도 폴더가 있으면 하위 달 폴더를 탐색함
            int month = cur_time.tm_mon + 1;
            while (month)
            {
                std::string path_month = path_year + "/" + std::to_string(month--);
                if (!boost::filesystem::exists(path_month))
                    continue;

                int day = cur_time.tm_mday;
                while (day)
                {
                    std::string path_day = path_month + "/" + std::to_string(day--) + ".json";
                    if (!boost::filesystem::exists(path_day))
                        continue;

                    str_buf.clear();
                    session_info.open(path_day);
                    if (session_info.is_open())
                        str_buf << session_info.rdbuf();
                    session_info.close();

                    boost::json::object chat_content;
                    chat_content["chat_date"] = std::format("{}-{}-{}", year, month + 1, day + 1);
                    chat_content["content"] = str_buf.str();
                    content_array.push_back(chat_content);

                    if (content_array.size() == 3)
                        goto CONTENT_ARRAY_IS_FULL;
                }
            }
        }

    CONTENT_ARRAY_IS_FULL:
        boost::json::object chat_obj;
        chat_obj["creator_id"] = creator_id;
        chat_obj["session_id"] = session_id;
        chat_obj["session_name"] = session_name;
        chat_obj["content"] = content_array;

        int width, height, channels;
        std::string image_path = chat_room_path + "/session_img.png";
        unsigned char *img = stbi_load(image_path.c_str(), &width, &height, &channels, 0);
        std::string img_buffer(reinterpret_cast<char const *>(img), width * height);
        chat_obj["session_img"] = EncodeBase64(img_buffer);

        chat_room_array.push_back(chat_obj);
    }

    boost::json::object chatroom_init_json;
    chatroom_init_json["chatroom_init_data"] = chat_room_array;

    // chatroom_init_json에 담긴 json을 client에 전송함
    m_request = StrToUtf8(boost::json::serialize(chatroom_init_json));

    TCPHeader header(CHATROOMLIST_INITIAL_TYPE, m_request.size());
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