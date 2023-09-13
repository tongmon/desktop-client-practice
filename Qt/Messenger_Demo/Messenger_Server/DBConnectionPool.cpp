#include "DBConnectionPool.hpp"

#include <regex>

DBConnectionPool::DBConnectionPool(int pool_size, const DBConnectionInfo &connection_info)
{
    m_connection_pool = std::make_unique<soci::connection_pool>(pool_size);

    std::string session_info = "host=%DB_HOST% port=%DB_PORT% dbname=%DB_NAME% user=%DB_USER% password=%DB_PASSWORD%";
    session_info = std::regex_replace(session_info, std::regex("%DB_HOST%"), connection_info.db_host);
    session_info = std::regex_replace(session_info, std::regex("%DB_PORT%"), connection_info.db_port);
    session_info = std::regex_replace(session_info, std::regex("%DB_NAME%"), connection_info.db_name);
    session_info = std::regex_replace(session_info, std::regex("%DB_USER%"), connection_info.db_user);
    session_info = std::regex_replace(session_info, std::regex("%DB_PASSWORD%"), connection_info.db_password);

    for (size_t i = 0; i != pool_size; ++i)
    {
        soci::session &sql = m_connection_pool->at(i);
        sql.open(*soci::factory_postgresql(), session_info);
    }
}

DBConnectionPool::~DBConnectionPool()
{
}
