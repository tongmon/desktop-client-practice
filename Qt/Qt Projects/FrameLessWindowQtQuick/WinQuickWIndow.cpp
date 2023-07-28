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
    m_hwnd = (HWND)m_window.winId();

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

    // 창 최소 크기 설정
    m_parent_native_window->SetMinimumSize(400, 300);

    // 창 최대 크기 설정
    // m_parent_native_window->setMaximumSize(1920, 1080);

    if (GetParentHandle())
    {
        // m_window.setProperty("_q_embedded_native_parent_handle", (WId)GetParentHandle());
        // SetWindowLong(m_hwnd, GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        // SetParent(m_hwnd, GetParentHandle());

        m_window.setParent(QWindow::fromWinId((WId)GetParentHandle()));
        QEvent e(QEvent::EmbeddingControl);
        QGuiApplication::sendEvent(&m_window, &e);
    }

    m_parent_native_window->child_window = &m_window;
    m_parent_native_window->child_hwnd = m_hwnd;

    m_qml_connector = std::make_shared<QmlConnectObj>(*this);
    m_window.installEventFilter(m_qml_connector.get());
    engine.rootContext()->setContextProperty("cppConnector", m_qml_connector.get());

    border_width *= m_window.devicePixelRatio();
    titlebar_height *= m_window.devicePixelRatio();

    m_window.setProperty("titlebarHeight", titlebar_height);

    SendMessage(GetParentHandle(), WM_SIZE, 0, 0);

    Show();
}

WinQuickWindow::~WinQuickWindow()
{
}

void WinQuickWindow::Show()
{
    ShowWindow(GetParentHandle(), true);
    BringWindowToTop(GetParentHandle());
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

    switch (msg->message)
    {
    // 최상위 창이 클릭으로 인해 해당 창으로 바뀌는 경우에 대한 로직
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN: {
        if (!m_window.isActive())
            SetFocus(GetParentHandle());
        break;
    }

    case WM_NCLBUTTONDOWN: {
        // if (!m_window.isActive())
        //     SetFocus(GetParentHandle());
        break;
    }

    case WM_SETFOCUS: {
        break;
    }

    case WM_CLOSE: {
        if (true) // 종료 불가한 상황이 있다면 true 대신에 교체
        {
            // 닫기 전에 부모창을 숨기고 닫음
            ShowWindow(GetParentHandle(), false);
            QGuiApplication::quit();
        }
        else
        {
            *result = 0;
            return true;
        }
        break;
    }

    case WM_SIZE: {
        WINDOWPLACEMENT wp;
        GetWindowPlacement(GetParentHandle(), &wp);

        // 최대화면 버튼을 체크하여 버튼 이미지를 복구 이미지로 변경
        // 노말 상태면 버튼을 체크 해제하여 버튼 이미지를 최대화 이미지로 변경
        m_window.findChild<QObject *>("maximumButton")->setProperty("checked", wp.showCmd == SW_MAXIMIZE ? true : false);
        break;
    }

    // Native 부모창에 가해지는 메시지를 제외하고는 모두 qml에서 받기에 qml에서 ALT + SPACE 혹은 F10이 트리거되는지를 봐야할 것이다.
    case WM_SYSKEYDOWN: {
        // ALT + SPACE 혹은 F10 시스템 메뉴 발동
        if (msg->wParam == VK_SPACE || (msg->wParam == SC_KEYMENU && msg->lParam == VK_SPACE))
            DefWindowProc(GetParentHandle(), msg->message, msg->wParam, msg->lParam);
        break;
    }

    // 윈도우 툴바 영역, 창 크기 조절 영역 검사
    case WM_NCHITTEST: {
        RECT window_rect;
        int x, y, global_x = GET_X_LPARAM(msg->lParam), global_y = GET_Y_LPARAM(msg->lParam);

        GetWindowRect(msg->hwnd, &window_rect);
        x = global_x - window_rect.left;
        y = global_y - window_rect.top;

        if (x >= border_width && x <= window_rect.right - window_rect.left - border_width && y >= border_width && y <= titlebar_height)
        {
            // qml에서 클릭되지 않고 윈도우에서 클릭 메시지를 직접 전달해야 하는 영역 검사
            // qml 함수를 C++에서 이용
            if (IsTitleBarClickEventAllowedZone(global_x, global_y))
                return false;

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

        break;
    }

    default:
        break;
    }

    return false;
}

// qml 윈도우용 eventFilter
bool QmlConnectObj::eventFilter(QObject *obj, QEvent *evt)
{
    // auto quick_window = reinterpret_cast<QQuickWindow *>(obj);

    // switch (evt->type())
    // {
    // case QEvent::FocusOut:
    //     break;
    // case QEvent::FocusIn:
    //     break;
    // default:
    //     break;
    // }

    return QObject::eventFilter(obj, evt);
}

// 최소화 버튼 클릭시 수행됨, qml에 cppConnector로 연동됨
void QmlConnectObj::onMinimizeButtonClicked()
{
    SendMessage(m_quick_window.GetParentHandle(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

// 최대화 버튼 클릭시 수행됨, qml에 cppConnector로 연동됨
void QmlConnectObj::onMaximizeButtonClicked()
{
    bool checked = m_quick_window.m_window.findChild<QObject *>("maximumButton")->property("checked").toBool();
    SendMessage(m_quick_window.GetParentHandle(), WM_SYSCOMMAND, checked ? SC_MAXIMIZE : SC_RESTORE, 0);
}

// 닫기 버튼 클릭시 수행됨, qml에 cppConnector로 연동됨
void QmlConnectObj::onCloseButtonClicked()
{
    if (true)
    {
        ShowWindow(m_quick_window.GetParentHandle(), false);
        SendMessage(m_quick_window.GetParentHandle(), WM_CLOSE, 0, 0);
    }
}