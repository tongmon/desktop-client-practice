#include "MainPageContext.hpp"
#include "NetworkDefinition.hpp"
#include "WinQuickWindow.hpp"

MainPageContext::MainPageContext(WinQuickWindow *window)
    : m_window{window}
{
}

MainPageContext::~MainPageContext()
{
}

void MainPageContext::trySendTextChat(const QString &room_id, const QString &content)
{
    return;
}

void MainPageContext::initialChatRoomList(const QString &user_id)
{
    auto &network_handle = m_window->GetNetworkHandle();

    int request_id = 0;
    network_handle.AsyncConnect(SERVER_IP, SERVER_PORT, request_id);

    std::string request = user_id.toStdString();
    TCPHeader header(CHATROOMLIST_INITIAL_TYPE, request.size());
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

            if (connection_type != CHATROOMLIST_INITIAL_TYPE)
            {
                network_handle.CloseRequest(session->GetID());
                return;
            }

            network_handle.AsyncRead(session->GetID(), data_size, [&network_handle, this](std::shared_ptr<Session> session) -> void {
                if (!session.get() || !session->IsValid())
                    return;

                network_handle.CloseRequest(session->GetID());
            });
        });
    });

    return;
}
