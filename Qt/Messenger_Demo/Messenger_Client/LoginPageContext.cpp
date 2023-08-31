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
    });
}
