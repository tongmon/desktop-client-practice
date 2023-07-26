#include "WinQuickWindow.hpp"
#include "WinNativeWindow.hpp"

#include <QMetaObject>
#include <QQmlContext>
#include <QQmlProperty>
#include <QUrl>
#include <dwmapi.h>
#include <stdexcept>

WinQuickWindow::WinQuickWindow(QQuickWindow &quick_window, QQmlApplicationEngine &engine)
    : m_window{quick_window}, QAbstractNativeEventFilter()
{
    m_parent_native_window = std::make_unique<WinNativeWindow>(1 * m_window.devicePixelRatio(),
                                                               1 * m_window.devicePixelRatio(),
                                                               1 * m_window.devicePixelRatio(),
                                                               1 * m_window.devicePixelRatio());

    // 초반 윈도우 크기, 위치 설정
    int window_x, window_y, window_width, window_height;
    window_x = 100;
    window_y = 100;
    window_width = 1024;
    window_height = 768;

    SetGeometry(window_x, window_y, window_width, window_height);

    if (GetParentHandle())
    {
        m_window.setProperty("_q_embedded_native_parent_handle", (WId)GetParentHandle());
        // SetWindowLong((HWND)m_window.winId(), GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        SetParent((HWND)m_window.winId(), GetParentHandle());
        QEvent e(QEvent::EmbeddingControl);
        QGuiApplication::sendEvent(&m_window, &e);
    }

    m_parent_native_window->child_window = &m_window;
    m_parent_native_window->child_hwnd = (HWND)m_window.winId();
    m_hwnd = (HWND)m_window.winId();

    m_qml_connector = std::make_shared<QmlConnectObj>(*this);
    m_window.installEventFilter(m_qml_connector.get());
    engine.rootContext()->setContextProperty("cppConnector", m_qml_connector.get());

    border_width *= m_window.devicePixelRatio();
    titlebar_height *= m_window.devicePixelRatio();

    m_window.setProperty("titlebarHeight", titlebar_height);

    SendMessage(GetParentHandle(), WM_SIZE, 0, 0);

    ShowWindow(GetParentHandle(), true);

    BringWindowToTop(m_hwnd);
}

WinQuickWindow::~WinQuickWindow()
{
}

void WinQuickWindow::Show()
{
    ShowWindow(GetParentHandle(), true);
}

void WinQuickWindow::Center()
{
}

void WinQuickWindow::ShowCentered()
{
}

void WinQuickWindow::SetGeometry(int x, int y, int w, int h)
{
    m_parent_native_window->SetGeometry(x * m_window.devicePixelRatio(), y * m_window.devicePixelRatio(), w * m_window.devicePixelRatio(), h * m_window.devicePixelRatio());
}

void WinQuickWindow::SetWidnowTitle(const QString &title)
{
}

HWND WinQuickWindow::GetParentHandle() const
{
    return m_parent_native_window->GetHandle();
}

HWND WinQuickWindow::GetHandle() const
{
    return m_hwnd;
}

bool WinQuickWindow::IsTitleBarClickEventAllowedZone(const int &x, const int &y)
{
    QVariant ret;
    QMetaObject::invokeMethod(&m_window,
                              "isTitleBarClickEventAllowedZone",
                              Q_RETURN_ARG(QVariant, ret),
                              Q_ARG(QVariant, x),
                              Q_ARG(QVariant, y));

    return ret.toBool();
}

bool WinQuickWindow::nativeEventFilter(const QByteArray &event_type, void *message, long *result)
{
    MSG *msg = (MSG *)message;

    // if (msg->message == WM_ACTIVATE)
    // {
    //     HWND foreground_window = GetForegroundWindow();
    //     static int active = -1;
    //     if (active != (foreground_window == GetParentHandle() || foreground_window == m_hwnd))
    //     {
    //         active = foreground_window == GetParentHandle() || foreground_window == m_hwnd;
    //         if ((active && foreground_window != m_hwnd) || foreground_window == m_hwnd)
    //         {
    //             qDebug() << "Quick to Foreground!";
    //             BringWindowToTop(GetParentHandle());
    //             BringWindowToTop(m_hwnd);
    //         }
    //     }
    // }

    if (msg->message == WM_SETFOCUS)
    {
        // Qt::FocusReason reason;
        // if (::GetKeyState(VK_LBUTTON) < 0 || ::GetKeyState(VK_RBUTTON) < 0)
        //     reason = Qt::MouseFocusReason;
        // else if (::GetKeyState(VK_SHIFT) < 0)
        //     reason = Qt::BacktabFocusReason;
        // else
        //     reason = Qt::TabFocusReason;
        // QFocusEvent e(QEvent::FocusIn, reason);
        // QGuiApplication::sendEvent(&m_window, &e);
    }

    // Only close if safeToClose clears()
    if (msg->message == WM_CLOSE)
    {
        if (true /* put your check for it if it safe to close your app here */) // eg, does the user need to save a document
        {
            // Safe to close, so hide the parent window
            ShowWindow(GetParentHandle(), false);

            // And then quit
            QGuiApplication::quit();
        }
        else
        {
            *result = 0; // Set the message to 0 to ignore it, and thus, don't actually close
            return true;
        }
    }

    // Double check WM_SIZE messages to see if the parent native window is maximized
    if (msg->message == WM_SIZE)
    {
        // if (p_Widget && p_Widget->GetMaximizeBtn())
        // {
        //     // Get the window state
        //     WINDOWPLACEMENT wp;
        //     GetWindowPlacement(m_ParentNativeWindowHandle, &wp);
        //
        //     // 최대화면 버튼을 체크하여 버튼 이미지를 복구 이미지로 변경
        //     // 노말 상태면 버튼을 체크 해제하여 버튼 이미지를 최대화 이미지로 변경
        //     p_Widget->GetMaximizeBtn()->setChecked(wp.showCmd == SW_MAXIMIZE ? true : false);
        // }
    }

    if (msg->message == WM_SYSKEYDOWN)
    {
        // ALT + SPACE or F10 system menu
        if (msg->wParam == VK_SPACE || (msg->wParam == SC_KEYMENU && msg->lParam == VK_SPACE))
            DefWindowProc(GetParentHandle(), msg->message, msg->wParam, msg->lParam);
    }

    // Pass NCHITTESTS on the window edges as determined by BORDERWIDTH & TOOLBARHEIGHT through to the parent native window
    if (msg->message == WM_NCHITTEST)
    {
        RECT window_rect;
        int x, y, global_x = GET_X_LPARAM(msg->lParam), global_y = GET_Y_LPARAM(msg->lParam);

        GetWindowRect(msg->hwnd, &window_rect);
        x = global_x - window_rect.left;
        y = global_y - window_rect.top;

        if (x >= border_width && x <= window_rect.right - window_rect.left - border_width && y >= border_width && y <= titlebar_height)
        {
            if (IsTitleBarClickEventAllowedZone(global_x, global_y))
                return false;

            // The mouse is over the toolbar area & is NOT over a child of the toolbar, so pass this message
            // through to the native window for HTCAPTION dragging
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x < border_width && y < border_width)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > window_rect.right - window_rect.left - border_width && y < border_width)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > window_rect.right - window_rect.left - border_width && y > window_rect.bottom - window_rect.top - border_width)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x < border_width && y > window_rect.bottom - window_rect.top - border_width)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x < border_width)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (y < border_width)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > window_rect.right - window_rect.left - border_width)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (y > window_rect.bottom - window_rect.top - border_width)
        {
            *result = HTTRANSPARENT;
            return true;
        }

        return false;
    }

    // if (msg->message == WM_NCLBUTTONDOWN)
    // {
    //     qDebug() << "nclbutton";
    // }

    if (msg->message == WM_LBUTTONDOWN ||
        msg->message == WM_RBUTTONDOWN ||
        msg->message == WM_MBUTTONDOWN)
    {
        // qDebug() << "MBtn Down";
        // BringWindowToTop(GetParentHandle());
        // BringWindowToTop(m_hwnd);
    }

    if (msg->message == WM_LBUTTONUP ||
        msg->message == WM_RBUTTONUP ||
        msg->message == WM_MBUTTONUP)
    {
        // qDebug() << "MBtn Up";

        // HWND top_window = nullptr;
        // GetTopWindow(top_window);
        //
        // if (top_window != m_hwnd)
        // {
        //     BringWindowToTop(GetParentHandle());
        //     BringWindowToTop(m_hwnd);
        // }
    }

    return false;
}

bool QmlConnectObj::eventFilter(QObject *obj, QEvent *evt)
{
    auto quick_window = reinterpret_cast<QQuickWindow *>(obj);

    // if (GetForegroundWindow() != (HWND)quick_window->winId())
    // {
    //     SetForegroundWindow((HWND)quick_window->winId());
    // }

    qDebug() << evt->type();

    switch (evt->type())
    {
        // case QEvent::FocusOut: {
        //     // if (GetForegroundWindow() == native_hwnd)
        //     //{
        //     SetForegroundWindow((HWND)quick_window->winId());
        //     //}
        //     break;
        // }
        //

        // case QEvent::FocusIn: {
        //     qDebug() << "Focus in";
        //     BringWindowToTop((HWND)quick_window->winId());
        //     break;
        // }

        //
        // case QEvent::FocusAboutToChange: {
        //     qDebug() << "Focust About to Change!";
        //     break;
        // }
    default:
        break;
    }
    return QObject::eventFilter(obj, evt);
}

void QmlConnectObj::OnMinimizeButtonClicked()
{
    qDebug() << "MinimizeButton Clicked";
}

void QmlConnectObj::OnMaximizeButtonClicked()
{
    qDebug() << "MaximizeButton Clicked";
}

void QmlConnectObj::OnCloseButtonClicked()
{
    qDebug() << "CloseButton Clicked";
}