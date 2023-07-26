#include "WinNativeWindow.hpp"

#include <QDebug>
#include <QGuiApplication>
#include <QMouseEvent>
#include <dwmapi.h>
#include <stdexcept>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtWinExtras>
#endif

WinNativeWindow::WinNativeWindow(const int x, const int y, const int width, const int height)
    : m_hwnd(nullptr)
{
    // The native window technically has a background color. You can set it here
    HBRUSH window_background = CreateSolidBrush(RGB(255, 255, 255));

    HINSTANCE h_instance = GetModuleHandle(nullptr);
    WNDCLASSEX wcx = {0};

    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.hInstance = h_instance;
    wcx.lpfnWndProc = WndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.lpszClassName = L"WindowClass";
    wcx.hbrBackground = window_background;
    wcx.hCursor = LoadCursor(h_instance, IDC_ARROW);

// 윈도우 아이콘 수정
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QPixmap appIcon(":/icon/ApplicationIcon.png");
    wcx.hIcon = QtWin::toHICON(appIcon);
#else
    QImage appIcon(":/icon/ApplicationIcon.png");
    wcx.hIcon = QImage::toHICON(appIcon);
#endif

    RegisterClassEx(&wcx);
    if (FAILED(RegisterClassEx(&wcx)))
    {
        throw std::runtime_error("Couldn't register window class");
    }

    // Create a native window with the appropriate style
    m_hwnd = CreateWindow(L"WindowClass", L"WindowTitle", aero_borderless, x, y, width, height, 0, 0, h_instance, nullptr);

    if (!m_hwnd)
        throw std::runtime_error("couldn't create window because of reasons");

    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    // 윈도우 주변 그림자 키기
    const MARGINS aero_shadow_on = {1, 1, 1, 1};
    ::DwmExtendFrameIntoClientArea(m_hwnd, &aero_shadow_on);

    SetWindowPos(m_hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

WinNativeWindow::~WinNativeWindow()
{
    // Hide the window & send the destroy message
    ShowWindow(m_hwnd, SW_HIDE);
    DestroyWindow(m_hwnd);
}

LRESULT CALLBACK WinNativeWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    WinNativeWindow *window = reinterpret_cast<WinNativeWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (!window)
        return DefWindowProc(hwnd, message, wparam, lparam);

    switch (message)
    {
    // ALT + SPACE or F10 system menu
    case WM_SYSCOMMAND: {
        if (wparam == VK_SPACE || (wparam == SC_KEYMENU && lparam == VK_SPACE))
        {
            HMENU menu = GetSystemMenu(hwnd, FALSE);
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
                GetWindowPlacement(hwnd, &wp);

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
                GetWindowRect(hwnd, &winrect);

                LPARAM cmd = TrackPopupMenu(menu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                                            // 창 크기 최대인 경우 듀얼 모니터 사용자에게서 팝 업 메뉴가 다른 모니터를 침범하는 경우가 있음
                                            // 이를 해결하기 위해 팝업 메뉴의 시작부를 윈도우 안쪽으로 살짝 밀어줌
                                            winrect.left + (wp.showCmd == SW_SHOWMAXIMIZED ? 8 : 0),
                                            winrect.top, NULL, hwnd, nullptr);

                if (cmd)
                    PostMessage(hwnd, WM_SYSCOMMAND, cmd, 0);
            }
            return 0;
        }
        break;
    }
    case WM_NCCALCSIZE: {
        // this kills the window frame and title bar we added with
        // WS_THICKFRAME and WS_CAPTION
        return 0;
    }

        // case WM_ACTIVATE: {
        //     if (wparam != WA_INACTIVE && child_hwnd)
        //     {
        //         SetForegroundWindow(child_hwnd);
        //     }
        //     break;
        // }

        // case WM_LBUTTONDOWN: {
        //     if (child_hwnd && child_hwnd != GetForegroundWindow())
        //     {
        //         SetForegroundWindow(hwnd);
        //         SetForegroundWindow(child_hwnd);
        //     }
        //     break;
        // }
        //

        // case WM_NCLBUTTONDOWN: {
        //     if (child_hwnd)
        //     {
        //         qDebug() << "ncButtonDown";
        //     }
        //     break;
        // }

    case WM_ACTIVATEAPP: {
        if (child_hwnd)
        {
            if (wparam)
            {
                qDebug() << "Actiate";

                BringWindowToTop(hwnd);
                BringWindowToTop(child_hwnd);
            }
            else
            {
                qDebug() << "Deactiate";
            }
        }
        break;
    }

    case WM_ACTIVATE:
    // If the parent window gets any close messages, send them over to QWinWidget and don't actually close here
    case WM_CLOSE: {
        if (child_hwnd)
        {
            // QGuiApplication::sendEvent(child_window, );
            SendMessage(child_hwnd, message, 0, 0);
            return 0;
        }
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    case WM_NCHITTEST: {
        // This value can be arbitrarily large as only intentionally-HTTRANSPARENT'd messages arrive here
        const LONG border_width = 8 * child_window->devicePixelRatio();
        RECT winrect;
        GetWindowRect(hwnd, &winrect);
        long x = GET_X_LPARAM(lparam);
        long y = GET_Y_LPARAM(lparam);

        // if (child_hwnd)
        //     SetForegroundWindow(child_hwnd);

        // bottom left corner
        if (x >= winrect.left && x < winrect.left + border_width &&
            y < winrect.bottom && y >= winrect.bottom - border_width)
        {
            return HTBOTTOMLEFT;
        }
        // bottom right corner
        if (x < winrect.right && x >= winrect.right - border_width &&
            y < winrect.bottom && y >= winrect.bottom - border_width)
        {
            return HTBOTTOMRIGHT;
        }
        // top left corner
        if (x >= winrect.left && x < winrect.left + border_width &&
            y >= winrect.top && y < winrect.top + border_width)
        {
            return HTTOPLEFT;
        }
        // top right corner
        if (x < winrect.right && x >= winrect.right - border_width &&
            y >= winrect.top && y < winrect.top + border_width)
        {
            return HTTOPRIGHT;
        }
        // left border
        if (x >= winrect.left && x < winrect.left + border_width)
        {
            return HTLEFT;
        }
        // right border
        if (x < winrect.right && x >= winrect.right - border_width)
        {
            return HTRIGHT;
        }
        // bottom border
        if (y < winrect.bottom && y >= winrect.bottom - border_width)
        {
            return HTBOTTOM;
        }
        // top border
        if (y >= winrect.top && y < winrect.top + border_width)
        {
            return HTTOP;
        }

        // If it wasn't a border but we still got the message, return HTCAPTION to allow click-dragging the window
        return HTCAPTION;
    }

    // When this native window changes size, it needs to manually resize the QWinWidget child
    case WM_SIZE: {
        RECT winrect;
        GetClientRect(hwnd, &winrect);

        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwnd, &wp);
        if (child_window)
        {
            if (wp.showCmd == SW_MAXIMIZE)
            {
                // Maximized window draw 8 pixels off screen
                child_window->setGeometry(8, 8,
                                          winrect.right / child_window->devicePixelRatio() - 16, winrect.bottom / child_window->devicePixelRatio() - 16);
            }
            else
            {
                child_window->setGeometry(0, 0, winrect.right / child_window->devicePixelRatio(), winrect.bottom / child_window->devicePixelRatio());
            }
        }

        break;
    }

    case WM_GETMINMAXINFO: {
        MINMAXINFO *minmax_info = (MINMAXINFO *)lparam;
        if (window->GetMinimunSizeInfo().required)
        {
            minmax_info->ptMinTrackSize.x = window->GetMinimumWidth();
            minmax_info->ptMinTrackSize.y = window->GetMinimumHeight();
        }

        if (window->GetMaximunSizeInfo().required)
        {
            minmax_info->ptMaxTrackSize.x = window->GetMaximumWidth();
            minmax_info->ptMaxTrackSize.y = window->GetMaximumHeight();
        }
        return 0;
    }
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

void WinNativeWindow::SetGeometry(const int x, const int y, const int width, const int height)
{
    MoveWindow(m_hwnd, x, y, width, height, 1);
}

void WinNativeWindow::SetMinimumSize(const int width, const int height)
{
    m_minimum_size.required = true;
    m_minimum_size.width = width;
    m_minimum_size.height = height;
}

int WinNativeWindow::GetMinimumWidth()
{
    return m_minimum_size.width;
}

int WinNativeWindow::GetMinimumHeight()
{
    return m_minimum_size.height;
}

void WinNativeWindow::SetMaximumSize(const int width, const int height)
{
    m_maximum_size.required = true;
    m_maximum_size.width = width;
    m_maximum_size.height = height;
}

int WinNativeWindow::GetMaximumWidth()
{
    return m_maximum_size.width;
}

int WinNativeWindow::GetMaximumHeight()
{
    return m_maximum_size.height;
}

WinNativeWindow::SizeType WinNativeWindow::GetMinimunSizeInfo()
{
    return m_minimum_size;
}

WinNativeWindow::SizeType WinNativeWindow::GetMaximunSizeInfo()
{
    return m_maximum_size;
}