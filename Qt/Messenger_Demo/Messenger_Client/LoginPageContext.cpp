#include "LoginPageContext.hpp"
#include "NetworkDefinition.hpp"
#include "TCPClient.hpp"
#include "WinQuickWindow.hpp"

#include <QMetaObject>

LoginPageContext::LoginPageContext(WinQuickWindow *window)
    : m_window{window}
{
}

LoginPageContext::~LoginPageContext()
{
}

std::string LoginPageContext::GetUserID()
{
    return m_user_id;
}

std::string LoginPageContext::GetUserPW()
{
    return m_user_pw;
}

void LoginPageContext::tryLogin(const QString &id, const QString &pw)
{
    auto &central_server = m_window->GetServerHandle();

    int request_id = central_server.MakeRequestID();
    central_server.AsyncConnect(SERVER_IP, SERVER_PORT, request_id);

    m_user_id = id.toStdString(), m_user_pw = pw.toStdString();

    std::string request = m_window->GetIPAddress() + "|" + std::to_string(m_window->GetPortNumber()) + "|" +
                          m_user_id + "|" + m_user_pw;

    TCPHeader header(LOGIN_CONNECTION_TYPE, request.size());
    request = header.GetHeaderBuffer() + request;

    central_server.AsyncWrite(request_id, request, [&central_server, this](std::shared_ptr<Session> session) -> void {
        if (!session.get() || !session->IsValid())
            return;

        central_server.AsyncRead(session->GetID(), TCP_HEADER_SIZE, [&central_server, this](std::shared_ptr<Session> session) -> void {
            if (!session.get() || !session->IsValid())
                return;

            TCPHeader header(session->GetResponse());

            auto connection_type = header.GetConnectionType();
            auto data_size = header.GetDataSize();

            if (connection_type != LOGIN_CONNECTION_TYPE)
            {
                central_server.CloseRequest(session->GetID());
                return;
            }

            central_server.AsyncRead(session->GetID(), data_size, [&central_server, this](std::shared_ptr<Session> session) -> void {
                if (!session.get() || !session->IsValid())
                    return;

                // 로그인 성공
                if (session->GetResponse()[0])
                    QMetaObject::invokeMethod(m_window->GetQuickWindow().findChild<QObject *>("loginPage"), "successLogin");
                else
                {
                    // 로그인 실패시 로직
                }

                central_server.CloseRequest(session->GetID());
            });
        });
    });
}
