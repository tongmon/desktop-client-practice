#include "LoginPageContext.hpp"
#include "WinQuickWindow.hpp"

#include <QMetaObject>

LoginPageContext::LoginPageContext(WinQuickWindow *window)
    : m_window{window}
{
}

LoginPageContext::~LoginPageContext()
{
}

void LoginPageContext::tryLogin(const QString &id, const QString &pw)
{
    auto &network_handle = m_window->GetNetworkHandle();

    network_handle.AsyncConnect("127.0.0.1", 3000, 0);

    std::string request = id.toStdString() + "|^|" + pw.toStdString() + "\n";
    network_handle.AsyncWrite(0, request, [&network_handle, this](std::shared_ptr<Session> session) -> void {
        if (session->m_ec != boost::system::errc::success)
            return;

        network_handle.AsyncReadUntil(0, [&network_handle, this](std::shared_ptr<Session> session) -> void {
            if (session->m_ec != boost::system::errc::success)
                return;

            // QObject *loader = m_window->GetQuickWindow().findChild<QObject *>("mainWindowLoader");
            // loader->setProperty("source", "qrc:/qml/MainPage.qml");

            QMetaObject::invokeMethod(&m_window->GetQuickWindow(), "successLogin");
        });
    });

    // std::shared_ptr<TCPClient> tcp_client(new TCPClient(1));
    //
    // std::string request = id.toStdString() + "|^|" + pw.toStdString() + "\n";
    //
    // tcp_client->AsyncRequestAndGetData(request, "127.0.0.1", 3000, 0,
    //                                    [this, tcp_client](unsigned int request_id, const std::string &response, const boost::system::error_code &ec) -> void {
    //                                        switch (ec.value())
    //                                        {
    //                                        case boost::system::errc::success: {
    //                                            if (response == "success")
    //                                            {
    //                                                QObject *loader = m_quick_window->findChild<QObject *>("mainWindowLoader");
    //                                                loader->setProperty("source", "qrc:/qml/MainPage.qml");
    //                                            }
    //                                            break;
    //                                        }
    //                                        case boost::asio::error::operation_aborted:
    //
    //                                            break;
    //                                        default:
    //                                            // result = ec.message();
    //                                            break;
    //                                        }
    //
    //                                        tcp_client->Close();
    //                                    });
}
