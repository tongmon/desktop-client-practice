#include "LoginPageContext.hpp"
#include "NetworkDefinition.hpp"
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

    std::string request = id.toStdString() + "|" + pw.toStdString() + "\n";

    network_handle.AsyncWrite(0, request, [&network_handle, this](std::shared_ptr<Session> session) -> void {
        if (!session.get() || session->m_ec != boost::system::errc::success)
            return;

        network_handle.AsyncRead(0, TCP_HEADER_SIZE, [&network_handle, this](std::shared_ptr<Session> session) -> void {
            if (!session.get() || session->m_ec != boost::system::errc::success)
                return;

            TCPHeader header(session->m_response);

            auto connection_type = header.GetConnectionType();
            auto data_size = header.GetDataSize();

            // auto t = session->m_response;
            //
            // network_handle.AsyncRead(0, 8, [&network_handle, this](std::shared_ptr<Session> session) -> void {
            //     if (!session.get() || session->m_ec != boost::system::errc::success)
            //         return;
            //
            //     auto t = session->m_response;
            //
            //     network_handle.CloseRequest(session->m_id);
            // });

            // QMetaObject::invokeMethod(m_window->GetQuickWindow().findChild<QObject *>("loginPage"), "successLogin");

            // network_handle.CloseRequest(session->m_id);
        });

        // network_handle.AsyncReadUntil(0, [&network_handle, this](std::shared_ptr<Session> session) -> void {
        //     if (!session.get() || session->m_ec != boost::system::errc::success)
        //     {
        //         // 통신 장애 있다고 메시지 창 띄우던지... 하는 로직 추가
        //         return;
        //     }
        //
        //     // QObject *loader = m_window->GetQuickWindow().findChild<QObject *>("mainWindowLoader");
        //     // loader->setProperty("source", "qrc:/qml/MainPage.qml");
        //
        //     QMetaObject::invokeMethod(m_window->GetQuickWindow().findChild<QObject *>("loginPage"), "successLogin");
        //
        //     network_handle.CloseRequest(session->m_id);
        // });
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
