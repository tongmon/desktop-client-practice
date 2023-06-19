#include "QWinHost.hpp"
#include <QEvent>
#include <qt_windows.h>

#define QT_WA(unicode, ansi) unicode

QWinHost::QWinHost(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f), wndproc(0), own_hwnd(false), hwnd(0)
{
    setAttribute(Qt::WA_NoBackground);
    setAttribute(Qt::WA_NoSystemBackground);
}

QWinHost::~QWinHost()
{
    if (wndproc)
    {
#if defined(GWLP_WNDPROC)
        QT_WA({ SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wndproc); }, { SetWindowLongPtrA(hwnd, GWLP_WNDPROC, (LONG_PTR)wndproc); })
#else
        QT_WA({ SetWindowLong(hwnd, GWL_WNDPROC, (LONG)wndproc); }, { SetWindowLongA(hwnd, GWL_WNDPROC, (LONG)wndproc); })
#endif
    }

    if (hwnd && own_hwnd)
        DestroyWindow(hwnd);
}

HWND QWinHost::createWindow(HWND parent, HINSTANCE instance)
{
    Q_UNUSED(parent);
    Q_UNUSED(instance);
    return 0;
}

void QWinHost::fixParent()
{
    if (!hwnd)
        return;
    if (!::IsWindow(hwnd))
    {
        hwnd = 0;
        return;
    }
    if (::GetParent(hwnd) == (HWND)winId())
        return;
    long style = GetWindowLong(hwnd, GWL_STYLE);
    if (style & WS_OVERLAPPED)
        return;
    ::SetParent(hwnd, (HWND)winId());
}

/*!
    Sets the native Win32 window to \a window. If \a window is not a child
    window of this widget, then it is reparented to become one. If \a window
    is not a child window (i.e. WS_OVERLAPPED is set), then this function does nothing.

    The lifetime of the window handle will be managed by Windows, QWinHost does not
    call DestroyWindow. To verify that the handle is destroyed when expected, handle
    WM_DESTROY in the window procedure.

    \sa window(), createWindow()
*/
void QWinHost::setWindow(HWND window)
{
    if (hwnd && own_hwnd)
        DestroyWindow(hwnd);

    hwnd = window;
    fixParent();

    own_hwnd = false;
}

/*!
    Returns the handle to the native Win32 window, or null if no
    window has been set or created yet.

    \sa setWindow(), createWindow()
*/
HWND QWinHost::window() const
{
    return hwnd;
}

void *getWindowProc(QWinHost *host)
{
    return host ? host->wndproc : 0;
}

LRESULT CALLBACK WinHostProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    QWinHost *widget = qobject_cast<QWinHost *>(QWidget::find((WId)::GetParent(hwnd)));
    WNDPROC oldproc = (WNDPROC)getWindowProc(widget);
    if (widget)
    {
        switch (msg)
        {
        case WM_LBUTTONDOWN:
            if (::GetFocus() != hwnd && (widget->focusPolicy() & Qt::ClickFocus))
            {
                widget->setFocus(Qt::MouseFocusReason);
            }
            break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            QT_WA({ SendMessage((HWND)widget->winId(), msg, wParam, lParam); }, { SendMessageA((HWND)widget->winId(), msg, wParam, lParam); })
            break;

        case WM_KEYDOWN:
            if (wParam == VK_TAB)
            {
                QT_WA({ SendMessage((HWND)widget->winId(), msg, wParam, lParam); }, { SendMessageA((HWND)widget->winId(), msg, wParam, lParam); })
            }
            break;

        default:
            break;
        }
    }

    QT_WA({
    if (oldproc)
        return CallWindowProc(oldproc, hwnd, msg, wParam, lParam);
    return DefWindowProc(hwnd,msg,wParam,lParam); }, {
    if (oldproc)
        return CallWindowProcA(oldproc, hwnd, msg, wParam, lParam);
    return DefWindowProcA(hwnd,msg,wParam,lParam); })
}

/*!
    \reimp
*/
bool QWinHost::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::Polish:
        if (!hwnd)
        {
            hwnd = createWindow((HWND)winId(), GetModuleHandle(0)); // createWindow((HWND)winId(), qWinAppInst());
            fixParent();
            own_hwnd = hwnd != 0;
        }
        if (hwnd && !wndproc && GetParent(hwnd) == (HWND)winId())
        {
#if defined(GWLP_WNDPROC)
            QT_WA({
                wndproc = (void*)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
                SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WinHostProc); }, {
                wndproc = (void*)GetWindowLongPtrA(hwnd, GWLP_WNDPROC);
                SetWindowLongPtrA(hwnd, GWLP_WNDPROC, (LONG_PTR)WinHostProc); })
#else
            QT_WA({
                wndproc = (void*)GetWindowLong(hwnd, GWL_WNDPROC);
                SetWindowLong(hwnd, GWL_WNDPROC, (LONG)WinHostProc); }, {
                wndproc = (void*)GetWindowLongA(hwnd, GWL_WNDPROC);
                SetWindowLongA(hwnd, GWL_WNDPROC, (LONG)WinHostProc); })
#endif

            LONG style;
            QT_WA({ style = GetWindowLong(hwnd, GWL_STYLE); }, { style = GetWindowLongA(hwnd, GWL_STYLE); })
            if (style & WS_TABSTOP)
                setFocusPolicy(Qt::FocusPolicy(focusPolicy() | Qt::StrongFocus));
        }
        break;
    case QEvent::WindowBlocked:
        if (hwnd)
            EnableWindow(hwnd, false);
        break;
    case QEvent::WindowUnblocked:
        if (hwnd)
            EnableWindow(hwnd, true);
        break;
    }
    return QWidget::event(e);
}

/*!
    \reimp
*/
void QWinHost::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    if (hwnd)
        SetWindowPos(hwnd, HWND_TOP, 0, 0, width(), height(), SWP_SHOWWINDOW);
}

/*!
    \reimp
*/
void QWinHost::focusInEvent(QFocusEvent *e)
{
    QWidget::focusInEvent(e);

    if (hwnd)
        ::SetFocus(hwnd);
}

/*!
    \reimp
*/
void QWinHost::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if (hwnd)
        SetWindowPos(hwnd, HWND_TOP, 0, 0, width(), height(), 0);
}

/*!
    \reimp
*/
bool QWinHost::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = (MSG *)message;
    switch (msg->message)
    {
    case WM_SETFOCUS:
        if (hwnd)
        {
            ::SetFocus(hwnd);
            return true;
        }
    default:
        break;
    }
    return QWidget::nativeEvent(eventType, message, result);
}
