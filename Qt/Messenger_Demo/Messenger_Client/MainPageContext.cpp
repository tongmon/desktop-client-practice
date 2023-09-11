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
