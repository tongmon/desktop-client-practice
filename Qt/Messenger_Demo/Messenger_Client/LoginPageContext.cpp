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

    int request_id = 0;
    network_handle.AsyncConnect("127.0.0.1", 3000, request_id);

    std::string request = id.toStdString() + "|" + pw.toStdString();
    TCPHeader header(LOGIN_CONNECTION_TYPE, request.size());
    request = header.GetHeaderBuffer() + request;

    network_handle.AsyncWrite(request_id, request, [&network_handle, this](std::shared_ptr<Session> session) -> void {
        if (!session.get() || !session->IsValid())
            return;

        network_handle.AsyncRead(session->GetID(), TCP_HEADER_SIZE, [&network_handle, this](std::shared_ptr<Session> session) -> void {
            if (!session.get() || !session->IsValid())
                return;

            TCPHeader header(session->GetResponse());

            auto connection_type = header.GetConnectionType();
            auto data_size = header.GetDataSize();

            if (connection_type != LOGIN_CONNECTION_TYPE)
            {
                network_handle.CloseRequest(session->GetID());
                return;
            }

            network_handle.AsyncRead(session->GetID(), data_size, [&network_handle, this](std::shared_ptr<Session> session) -> void {
                if (!session.get() || !session->IsValid())
                    return;

                if (session->GetResponse()[0]) // 로그인 성공
                    QMetaObject::invokeMethod(m_window->GetQuickWindow().findChild<QObject *>("loginPage"), "successLogin");
                else
                {
                    // 로그인 실패시 로직
                }

                network_handle.CloseRequest(session->GetID());
            });
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
