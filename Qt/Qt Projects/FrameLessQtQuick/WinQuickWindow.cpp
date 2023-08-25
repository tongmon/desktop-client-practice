#include "WinQuickWindow.hpp"

#include <QMetaObject>
#include <Windows.h>
#include <Windowsx.h>
#include <dwmapi.h>
#include <memory>
#include <stdexcept>

WinQuickWindow::WinQuickWindow(QQuickWindow *quick_window)
    : m_quick_window{quick_window}
{
    SetQuickWindow(quick_window);
}

WinQuickWindow::~WinQuickWindow()
{
}

HWND WinQuickWindow::GetHandle()
{
    return m_hwnd;
}

bool WinQuickWindow::SetQuickWindow(QQuickWindow *quick_window)
{
    if (!quick_window)
        return false;

    m_quick_window = quick_window;
    m_hwnd = (HWND)m_quick_window->winId();
    m_resize_border_width = m_quick_window->property("resizeBorderWidth").toInt() * m_quick_window->devicePixelRatio();

    QObject::connect(m_quick_window, &QQuickWindow::screenChanged, this, &WinQuickWindow::OnScreenChanged);

    // 윈도우 그림자 설정
    const MARGINS aero_shadow_on = {1, 1, 1, 1};
    ::DwmExtendFrameIntoClientArea(m_hwnd, &aero_shadow_on);

    return true;
}

// 듀얼 모니터 이상에서 현재 화면이 바뀔때 프레임 창 렌더링이 잘못되는 현상 방지
void WinQuickWindow::OnScreenChanged(QScreen *screen)
{
    SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

bool WinQuickWindow::eventFilter(QObject *obj, QEvent *evt)
{
    switch (evt->type())
    {
    // case QEvent::Enter:
    //     break;
    // case QEvent::Leave:
    //     break;
    default:
        break;
    }

    return QObject::eventFilter(obj, evt);
}

bool WinQuickWindow::nativeEventFilter(const QByteArray &event_type, void *message, long *result)
{
    MSG *msg = (MSG *)message;

    switch (msg->message)
    {
    // 윈도우 기본 프레임 제거
    case WM_NCCALCSIZE: {
        if (msg->lParam)
        {
            WINDOWPLACEMENT wp;
            GetWindowPlacement(m_hwnd, &wp);

            if (wp.showCmd == SW_MAXIMIZE)
            {
                NCCALCSIZE_PARAMS *sz = (NCCALCSIZE_PARAMS *)msg->lParam;
                sz->rgrc[0].left += 8;
                sz->rgrc[0].top += 8;
                sz->rgrc[0].right -= 8;
                sz->rgrc[0].bottom -= 8;

                m_quick_window->findChild<QObject *>("maximumButton")->setProperty("checked", true);
            }
            else if (wp.showCmd == SW_NORMAL)
                m_quick_window->findChild<QObject *>("maximumButton")->setProperty("checked", false);
        }
        return true;
    }

    // 클릭 처리
    case WM_NCHITTEST: {
        RECT winrect;
        GetWindowRect(msg->hwnd, &winrect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        if (x >= winrect.left && x < winrect.left + m_resize_border_width &&
            y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
        {
            *result = HTBOTTOMLEFT;
            return true;
        }

        if (x < winrect.right && x >= winrect.right - m_resize_border_width &&
            y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
        {
            *result = HTBOTTOMRIGHT;
            return true;
        }

        if (x >= winrect.left && x < winrect.left + m_resize_border_width &&
            y >= winrect.top && y < winrect.top + m_resize_border_width)
        {
            *result = HTTOPLEFT;
            return true;
        }

        if (x < winrect.right && x >= winrect.right - m_resize_border_width &&
            y >= winrect.top && y < winrect.top + m_resize_border_width)
        {
            *result = HTTOPRIGHT;
            return true;
        }

        if (x >= winrect.left && x < winrect.left + m_resize_border_width)
        {
            *result = HTLEFT;
            return true;
        }

        if (x < winrect.right && x >= winrect.right - m_resize_border_width)
        {
            *result = HTRIGHT;
            return true;
        }

        if (y < winrect.bottom && y >= winrect.bottom - m_resize_border_width)
        {
            *result = HTBOTTOM;
            return true;
        }

        if (y >= winrect.top && y < winrect.top + m_resize_border_width)
        {
            *result = HTTOP;
            return true;
        }

        *result = HTTRANSPARENT;
        break;
    }

    // 시스템 메뉴 처리
    case WM_SYSCOMMAND: {
        if (msg->wParam == VK_SPACE || (msg->wParam == SC_KEYMENU && msg->lParam == VK_SPACE))
        {
            HMENU menu = GetSystemMenu(m_hwnd, FALSE);
            if (menu)
            {
                MENUITEMINFO mii;
                mii.cbSize = sizeof(MENUITEMINFO);
                mii.fMask = MIIM_STATE;
                mii.fType = 0;

                mii.fState = MF_ENABLED;
                SetMenuItemInfo(menu, SC_RESTORE, FALSE, &mii);
                SetMenuItemInfo(menu, SC_SIZE, FALSE, &mii);
                SetMenuItemInfo(menu, SC_MOVE, FALSE, &mii);
                SetMenuItemInfo(menu, SC_MAXIMIZE, FALSE, &mii);
                SetMenuItemInfo(menu, SC_MINIMIZE, FALSE, &mii);

                mii.fState = MF_GRAYED;

                WINDOWPLACEMENT wp;
                GetWindowPlacement(m_hwnd, &wp);

                switch (wp.showCmd)
                {
                case SW_SHOWMAXIMIZED:
                    SetMenuItemInfo(menu, SC_SIZE, FALSE, &mii);
                    SetMenuItemInfo(menu, SC_MOVE, FALSE, &mii);
                    SetMenuItemInfo(menu, SC_MAXIMIZE, FALSE, &mii);
                    SetMenuDefaultItem(menu, SC_CLOSE, FALSE);
                    break;
                case SW_SHOWMINIMIZED:
                    SetMenuItemInfo(menu, SC_MINIMIZE, FALSE, &mii);
                    SetMenuDefaultItem(menu, SC_RESTORE, FALSE);
                    break;
                case SW_SHOWNORMAL:
                    SetMenuItemInfo(menu, SC_RESTORE, FALSE, &mii);
                    SetMenuDefaultItem(menu, SC_CLOSE, FALSE);
                    break;
                }

                RECT winrect;
                GetWindowRect(m_hwnd, &winrect);

                LPARAM cmd = TrackPopupMenu(menu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                                            // 창 크기 최대인 경우 듀얼 모니터 사용자에게서 팝 업 메뉴가 다른 모니터를 침범하는 경우가 있음
                                            // 이를 해결하기 위해 팝업 메뉴의 시작부를 윈도우 안쪽으로 살짝 밀어줌
                                            winrect.left + (wp.showCmd == SW_SHOWMAXIMIZED ? 8 : 0),
                                            winrect.top, NULL, m_hwnd, nullptr);

                if (cmd)
                    PostMessage(m_hwnd, WM_SYSCOMMAND, cmd, 0);
            }
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

// 최소화 버튼 클릭시 수행됨, qml에 cppConnector로 연동됨
void WinQuickWindow::onMinimizeButtonClicked()
{
    SendMessage(m_hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

// 최대화 버튼 클릭시 수행됨, qml에 cppConnector로 연동됨
void WinQuickWindow::onMaximizeButtonClicked()
{
    bool checked = m_quick_window->findChild<QObject *>("maximumButton")->property("checked").toBool();
    SendMessage(m_hwnd, WM_SYSCOMMAND, checked ? SC_MAXIMIZE : SC_RESTORE, 0);
}

// 닫기 버튼 클릭시 수행됨, qml에 cppConnector로 연동됨
void WinQuickWindow::onCloseButtonClicked()
{
    SendMessage(m_hwnd, WM_CLOSE, 0, 0);
}