#ifndef HEADER__FILE__MESSENGERSERVICE
#define HEADER__FILE__MESSENGERSERVICE

#include "Service.hpp"

class MessengerService : public Service
{
    std::string m_request;
    std::string m_server_request;
    boost::asio::streambuf m_server_request_buf;

  public:
    MessengerService(std::shared_ptr<TCPClient> peer, std::shared_ptr<boost::asio::ip::tcp::socket> sock);
    ~MessengerService();
    void StartHandling();
};

#endif /* HEADER__FILE__MESSENGERSERVICE */
