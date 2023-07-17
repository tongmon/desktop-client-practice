#include "QWinWidget.h"
#include <QApplication>
#include <QEvent>
#include <QFocusEvent>
#include <QWindow>
#include <qt_windows.h>

/*!
    \class QWinWidget qwinwidget.h
    \brief The QWinWidget class is a Qt widget that can be child of a
    native Win32 widget.

    The QWinWidget class is the bridge between an existing application
    user interface developed using native Win32 APIs or toolkits like
    MFC, and Qt based GUI elements.

    Using QWinWidget as the parent of QDialogs will ensure that
    modality, placement and stacking works properly throughout the
    entire application. If the child widget is a top level window that
    uses the \c WDestructiveClose flag, QWinWidget will destroy itself
    when the child window closes down.

    Applications moving to Qt can use QWinWidget to add new
    functionality, and gradually replace the existing interface.
*/

QWinWidget::QWinWidget()
    : QWidget(nullptr),
      m_Layout(),
      p_Widget(nullptr),
      m_ParentNativeWindowHandle(nullptr),
      _prevFocus(nullptr),
      _reenableParent(false)
{

    // Create a native window and give it geometry values * devicePixelRatio for HiDPI support
    p_ParentWinNativeWindow = new WinNativeWindow(1 * window()->devicePixelRatio(), 1 * window()->devicePixelRatio(), 1 * window()->devicePixelRatio(), 1 * window()->devicePixelRatio());

    // 최소화 창의 크기를 고정하고 싶다면 밑 부분 주석 해제
    // p_ParentWinNativeWindow->setMinimumSize(1024 * window()->devicePixelRatio(), 768 * window()->devicePixelRatio());

    // 최대화 창의 크기를 고정하고 싶다면 밑 부분 주석 해제
    // p_ParentWinNativeWindow->setMaximumSize(1024 * window()->devicePixelRatio(), 768 * window()->devicePixelRatio());

    // Save the native window handle for shorthand use
    m_ParentNativeWindowHandle = p_ParentWinNativeWindow->hWnd;
    Q_ASSERT(m_ParentNativeWindowHandle);

    // Create the child window & embed it into the native one
    if (m_ParentNativeWindowHandle)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        setProperty("_q_embedded_native_parent_handle", (WId)m_ParentNativeWindowHandle);
        SetWindowLong((HWND)winId(), GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

        SetParent((HWND)winId(), m_ParentNativeWindowHandle);
        QEvent e(QEvent::EmbeddingControl);
        QApplication::sendEvent(this, &e);
    }

    // Pass along our window handle & widget pointer to WinFramelessWidget so we can exchange messages
    p_ParentWinNativeWindow->childWindow = (HWND)winId();
    p_ParentWinNativeWindow->childWidget = this;

    // Clear margins & spacing & add the layout to prepare for the MainAppWidget
    setContentsMargins(0, 0, 0, 0);
    setLayout(&m_Layout);
    m_Layout.setContentsMargins(0, 0, 0, 0);
    m_Layout.setSpacing(0);

    // Create the true app widget
    p_Widget = new Widget(this);

    m_Layout.addWidget(p_Widget);
    p_Widget->setParent(this, Qt::Widget);
    p_Widget->setVisible(true);

    // Update the BORDERWIDTH value if needed for HiDPI displays
    BORDERWIDTH = BORDERWIDTH * window()->devicePixelRatio();

    if (p_Widget->titleBar)
    {
        TOOLBARHEIGHT = p_Widget->titleBar->height() * window()->devicePixelRatio();
    }

    // You need to keep the native window in sync with the Qt window & children, so wire min/max/close buttons to
    // slots inside of QWinWidget. QWinWidget can then talk with the native window as needed
    if (p_Widget->minimizeButton)
    {
        connect(p_Widget->minimizeButton, &QPushButton::clicked, this, &QWinWidget::onMinimizeButtonClicked);
    }
    if (p_Widget->maximizeButton)
    {
        connect(p_Widget->maximizeButton, &QPushButton::clicked, this, &QWinWidget::onMaximizeButtonClicked);
    }
    if (p_Widget->closeButton)
    {
        connect(p_Widget->closeButton, &QPushButton::clicked, this, &QWinWidget::onCloseButtonClicked);
    }

    // Send the parent native window a WM_SIZE message to update the widget size
    SendMessage(m_ParentNativeWindowHandle, WM_SIZE, 0, 0);
}

/*!
    Destroys this object, freeing all allocated resources.
*/
QWinWidget::~QWinWidget()
{
}

/*!
    Returns the handle of the native Win32 parent window.
*/
HWND QWinWidget::getParentWindow() const
{
    return m_ParentNativeWindowHandle;
}

void QWinWidget::childEvent(QChildEvent *e)
{
    QObject *obj = e->child();
    if (obj->isWidgetType())
    {
        if (e->added())
        {
            if (obj->isWidgetType())
            {
                obj->installEventFilter(this);
            }
        }
        else if (e->removed() && _reenableParent)
        {
            _reenableParent = false;
            EnableWindow(m_ParentNativeWindowHandle, true);
            obj->removeEventFilter(this);
        }
    }
    QWidget::childEvent(e);
}

/*! \internal */
void QWinWidget::saveFocus()
{
    if (!_prevFocus)
        _prevFocus = ::GetFocus();
    if (!_prevFocus)
        _prevFocus = getParentWindow();
}

/*!
    Shows this widget. Overrides QWidget::show().

    \sa showCentered()
*/
void QWinWidget::show()
{
    ShowWindow(m_ParentNativeWindowHandle, true);
    saveFocus();
    QWidget::show();
}

/*!
    Centers this widget over the native parent window. Use this
    function to have Qt toplevel windows (i.e. dialogs) positioned
    correctly over their native parent windows.

    \code
    QWinWidget qwin(hParent);
    qwin.center();

    QMessageBox::information(&qwin, "Caption", "Information Text");
    \endcode

    This will center the message box over the client area of hParent.
*/
void QWinWidget::center()
{
    const QWidget *child = findChild<QWidget *>();
    if (child && !child->isWindow())
    {
        qWarning("QWinWidget::center: Call this function only for QWinWidgets with toplevel children");
    }
    RECT r;
    GetWindowRect(m_ParentNativeWindowHandle, &r);
    setGeometry((r.right - r.left) / 2 + r.left, (r.bottom - r.top) / 2 + r.top, 0, 0);
}

/*!
    \obsolete

    Call center() instead.
*/
void QWinWidget::showCentered()
{
    center();
    show();
}

void QWinWidget::setGeometry(int x, int y, int w, int h)
{
    p_ParentWinNativeWindow->setGeometry(x * window()->devicePixelRatio(), y * window()->devicePixelRatio(), w * window()->devicePixelRatio(), h * window()->devicePixelRatio());
}

/*!
    Sets the focus to the window that had the focus before this widget
    was shown, or if there was no previous window, sets the focus to
    the parent window.
*/
void QWinWidget::resetFocus()
{
    if (_prevFocus)
        ::SetFocus(_prevFocus);
    else
        ::SetFocus(getParentWindow());
}

// Tell the parent native window to minimize
void QWinWidget::onMinimizeButtonClicked()
{
    SendMessage(m_ParentNativeWindowHandle, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

// Tell the parent native window to maximize or restore as appropriate
void QWinWidget::onMaximizeButtonClicked()
{
    // 최대화 버튼을 최대화 상태에서 누르면 복구, 기본 상태에서 누르면 최대화
    SendMessage(m_ParentNativeWindowHandle, WM_SYSCOMMAND, p_Widget->maximizeButton->isChecked() ? SC_MAXIMIZE : SC_RESTORE, 0);

    // hover 상태 남아있는 현상 제거
    p_Widget->maximizeButton->setAttribute(Qt::WA_UnderMouse, false);
}

void QWinWidget::onCloseButtonClicked()
{
    if (true /* put your check for it if it safe to close your app here */) // eg, does the user need to save a document
    {
        // Safe to close, so hide the parent window
        ShowWindow(m_ParentNativeWindowHandle, false);

        // And then quit
        QApplication::quit();
    }
    else
    {
        // Do nothing, and thus, don't actually close the window
    }
}

bool QWinWidget::nativeEvent(const QByteArray &, void *message, long *result)
{
    MSG *msg = (MSG *)message;

    if (msg->message == WM_SETFOCUS)
    {
        Qt::FocusReason reason;
        if (::GetKeyState(VK_LBUTTON) < 0 || ::GetKeyState(VK_RBUTTON) < 0)
            reason = Qt::MouseFocusReason;
        else if (::GetKeyState(VK_SHIFT) < 0)
            reason = Qt::BacktabFocusReason;
        else
            reason = Qt::TabFocusReason;
        QFocusEvent e(QEvent::FocusIn, reason);
        QApplication::sendEvent(this, &e);
    }

    // Only close if safeToClose clears()
    if (msg->message == WM_CLOSE)
    {
        if (true /* put your check for it if it safe to close your app here */) // eg, does the user need to save a document
        {
            // Safe to close, so hide the parent window
            ShowWindow(m_ParentNativeWindowHandle, false);

            // And then quit
            QApplication::quit();
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
        if (p_Widget && p_Widget->maximizeButton)
        {
            // Get the window state
            WINDOWPLACEMENT wp;
            GetWindowPlacement(m_ParentNativeWindowHandle, &wp);

            // 최대화면 버튼을 체크하여 버튼 이미지를 복구 이미지로 변경
            // 노말 상태면 버튼을 체크 해제하여 버튼 이미지를 최대화 이미지로 변경
            p_Widget->maximizeButton->setChecked(wp.showCmd == SW_MAXIMIZE ? true : false);
        }
    }

    if (msg->message == WM_SYSKEYDOWN)
    {
        // ALT + SPACE or F10 system menu
        if (msg->wParam == VK_SPACE || (msg->wParam == SC_KEYMENU && msg->lParam == VK_SPACE))
            DefWindowProc(m_ParentNativeWindowHandle, msg->message, msg->wParam, msg->lParam);
    }

    // Pass NCHITTESTS on the window edges as determined by BORDERWIDTH & TOOLBARHEIGHT through to the parent native window
    if (msg->message == WM_NCHITTEST)
    {
        RECT WindowRect;
        int x, y;

        GetWindowRect(msg->hwnd, &WindowRect);
        x = GET_X_LPARAM(msg->lParam) - WindowRect.left;
        y = GET_Y_LPARAM(msg->lParam) - WindowRect.top;

        if (x >= BORDERWIDTH && x <= WindowRect.right - WindowRect.left - BORDERWIDTH && y >= BORDERWIDTH && y <= TOOLBARHEIGHT)
        {
            if (p_Widget->titleBar)
            {
                // 타이틀바에서 클릭 제외 영역 검사
                if (!p_Widget->isClickEventAllowedZone())
                {
                    // 드래깅 동작 진행
                    *result = HTTRANSPARENT;
                    return true;
                }

                // If the mouse is over top of the toolbar area BUT is actually positioned over a child widget of the toolbar,
                // Then we don't want to enable dragging. This allows for buttons in the toolbar, eg, a Maximize button, to keep the mouse interaction
                if (QApplication::widgetAt(QCursor::pos()) != p_Widget->titleBar)
                    return false;
            }

            // The mouse is over the toolbar area & is NOT over a child of the toolbar, so pass this message
            // through to the native window for HTCAPTION dragging
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x < BORDERWIDTH && y < BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH && y < BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH && y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x < BORDERWIDTH && y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x < BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (y < BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (x > WindowRect.right - WindowRect.left - BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }
        else if (y > WindowRect.bottom - WindowRect.top - BORDERWIDTH)
        {
            *result = HTTRANSPARENT;
            return true;
        }

        return false;
    }

    return false;
}

bool QWinWidget::eventFilter(QObject *o, QEvent *e)
{
    QWidget *w = (QWidget *)o;

    switch (e->type())
    {
    case QEvent::WindowDeactivate:
        if (w->isModal() && w->isHidden())
            BringWindowToTop(m_ParentNativeWindowHandle);
        break;

    case QEvent::Hide:
        if (_reenableParent)
        {
            EnableWindow(m_ParentNativeWindowHandle, true);
            _reenableParent = false;
        }
        resetFocus();

        if (w->testAttribute(Qt::WA_DeleteOnClose) && w->isWindow())
            deleteLater();
        break;

    case QEvent::Show:
        if (w->isWindow())
        {
            saveFocus();
            hide();
            if (w->isModal() && !_reenableParent)
            {
                EnableWindow(m_ParentNativeWindowHandle, false);
                _reenableParent = true;
            }
        }
        break;

    case QEvent::Close: {
        ::SetActiveWindow(m_ParentNativeWindowHandle);
        if (w->testAttribute(Qt::WA_DeleteOnClose))
            deleteLater();
        break;
    }
    default:
        break;
    }

    return QWidget::eventFilter(o, e);
}

void QWinWidget::focusInEvent(QFocusEvent *e)
{
    QWidget *candidate = this;

    switch (e->reason())
    {
    case Qt::TabFocusReason:
    case Qt::BacktabFocusReason:
        while (!(candidate->focusPolicy() & Qt::TabFocus))
        {
            candidate = candidate->nextInFocusChain();
            if (candidate == this)
            {
                candidate = 0;
                break;
            }
        }
        if (candidate)
        {
            candidate->setFocus(e->reason());
            if (e->reason() == Qt::BacktabFocusReason || e->reason() == Qt::TabFocusReason)
            {
                candidate->setAttribute(Qt::WA_KeyboardFocusChange);
                candidate->window()->setAttribute(Qt::WA_KeyboardFocusChange);
            }
            if (e->reason() == Qt::BacktabFocusReason)
                QWidget::focusNextPrevChild(false);
        }
        break;
    default:
        break;
    }
}

bool QWinWidget::focusNextPrevChild(bool next)
{
    QWidget *curFocus = focusWidget();
    if (!next)
    {
        if (!curFocus->isWindow())
        {
            QWidget *nextFocus = curFocus->nextInFocusChain();
            QWidget *prevFocus = 0;
            QWidget *topLevel = 0;
            while (nextFocus != curFocus)
            {
                if (nextFocus->focusPolicy() & Qt::TabFocus)
                {
                    prevFocus = nextFocus;
                    topLevel = 0;
                }
                nextFocus = nextFocus->nextInFocusChain();
            }

            if (!topLevel)
            {
                return QWidget::focusNextPrevChild(false);
            }
        }
    }
    else
    {
        QWidget *nextFocus = curFocus;
        while (1 && nextFocus != 0)
        {
            nextFocus = nextFocus->nextInFocusChain();
            if (nextFocus->focusPolicy() & Qt::TabFocus)
            {
                return QWidget::focusNextPrevChild(true);
            }
        }
    }

    ::SetFocus(m_ParentNativeWindowHandle);

    return true;
}
