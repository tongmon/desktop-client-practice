#ifndef HEADER__FILE__MESSENGERSERVICE
#define HEADER__FILE__MESSENGERSERVICE

#include <soci/postgresql/soci-postgresql.h>
#include <soci/soci.h>

#include "Service.hpp"

class MessengerService : public Service
{
    std::string m_request;
    std::string m_client_request;
    boost::asio::streambuf m_client_request_buf;

    std::unique_ptr<soci::session> m_sql;

    void LoginHandling();
    void MessageHandling();

  public:
    MessengerService(std::shared_ptr<boost::asio::ip::tcp::socket> sock);
    ~MessengerService();
    void StartHandling();
};

#endif /* HEADER__FILE__MESSENGERSERVICE */
