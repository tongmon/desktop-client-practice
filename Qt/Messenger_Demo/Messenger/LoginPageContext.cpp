#include "LoginPageContext.hpp"

LoginPageContext::LoginPageContext()
{
}

LoginPageContext::~LoginPageContext()
{
}

bool LoginPageContext::tryLogin(const QString &id, const QString &pw)
{
    TCPClient tcp_client(1);

    std::string request = id.toStdString() + "|^|" + pw.toStdString(), result;

    tcp_client.AsyncRequestAndGetData(request, "127.0.1.0", 8080, 0,
                                      [&result](unsigned int request_id, const std::string &response, const boost::system::error_code &ec) -> void {
                                          switch (ec.value())
                                          {
                                          case boost::system::errc::success:
                                              result = response;
                                              break;
                                          case boost::asio::error::operation_aborted:
                                              result = "aborted";
                                              break;
                                          default:
                                              result = ec.message();
                                              break;
                                          }
                                      });

    tcp_client.Close();

    return result == "success";
}
