#ifndef HEADER__FILE__DBCONNECTIONPOOL
#define HEADER__FILE__DBCONNECTIONPOOL

#include <atomic>
#include <memory>
#include <mutex>
#include <soci/postgresql/soci-postgresql.h>
#include <soci/soci.h>

struct DBConnectionInfo
{
    std::string db_host;
    std::string db_port;
    std::string db_name;
    std::string db_user;
    std::string db_password;
};

// TCPServer와의 결합은 없고 파생 Service에 대한 결합만 필요하기에 싱글턴으로 구성함
// DB 연결 풀을 담당함
class DBConnectionPool
{
    static inline std::atomic<std::shared_ptr<DBConnectionPool>> instance = nullptr;
    static inline std::mutex mut;

    struct Deleter
    {
        void operator()(DBConnectionPool *ptr)
        {
            delete ptr;
            instance.store(nullptr);
        }
    };
    friend Deleter;

    std::unique_ptr<soci::connection_pool> m_connection_pool;

    DBConnectionPool(int pool_size, const DBConnectionInfo &connection_info);
    ~DBConnectionPool();

  public:
    static soci::connection_pool &get(int pool_size = 8, const DBConnectionInfo &connection_info = {})
    {
        if (!instance.load(std::memory_order_acquire))
        {
            std::lock_guard<std::mutex> lock(mut);
            if (!instance.load(std::memory_order_relaxed))
                instance.store(std::shared_ptr<DBConnectionPool>(new DBConnectionPool(pool_size, connection_info), Deleter{}), std::memory_order_release);
        }
        return *instance.load(std::memory_order_relaxed)->m_connection_pool;
    }

    DBConnectionPool(DBConnectionPool const &) = delete;
    DBConnectionPool(DBConnectionPool &&) = delete;
    DBConnectionPool &operator=(DBConnectionPool const &) = delete;
    DBConnectionPool &operator=(DBConnectionPool &&) = delete;
};

#endif /* HEADER__FILE__DBCONNECTIONPOOL */
