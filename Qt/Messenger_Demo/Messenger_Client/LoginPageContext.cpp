#include "LoginPageContext.hpp"
#include "TCPClient.hpp"
#include <QQuickWindow>

LoginPageContext::LoginPageContext(QQuickWindow *quick_window)
    : m_quick_window{quick_window}
{
}

LoginPageContext::~LoginPageContext()
{
}

void LoginPageContext::tryLogin(const QString &id, const QString &pw)
{
    std::shared_ptr<TCPClient> tcp_client(new TCPClient(1));

    std::string request = id.toStdString() + "|^|" + pw.toStdString() + "\n";

    tcp_client->AsyncRequestAndGetData(request, "127.0.0.1", 3000, 0,
                                       [this, tcp_client](unsigned int request_id, const std::string &response, const boost::system::error_code &ec) -> void {
                                           switch (ec.value())
                                           {
                                           case boost::system::errc::success: {
                                               if (response == "success")
                                               {
                                                   QObject *loader = m_quick_window->findChild<QObject *>("mainWindowLoader");
                                                   loader->setProperty("source", "qrc:/qml/MainPage.qml");
                                               }
                                               break;
                                           }
                                           case boost::asio::error::operation_aborted:

                                               break;
                                           default:
                                               // result = ec.message();
                                               break;
                                           }

                                           tcp_client->Close();
                                       });
}
