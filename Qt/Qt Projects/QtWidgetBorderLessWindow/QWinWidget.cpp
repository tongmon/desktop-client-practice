#include "QWinWidget.hpp"
#include <QApplication>
#include <QEvent>
#include <QFocusEvent>
#include <QWindow>
#include <qpa/qplatformnativeinterface.h>
#include <qt_windows.h>

#define QT_WA(unicode, ansi) unicode

QWinWidget::QWinWidget(HWND hParentWnd, QObject *parent, Qt::WindowFlags f)
    : QWidget(0, f), hParent(hParentWnd), prevFocus(0), reenable_parent(false)
{
    if (parent)
        QObject::setParent(parent);

    init();
}

void QWinWidget::init()
{
    Q_ASSERT(hParent);

    if (hParent)
    {
        // make the widget window style be WS_CHILD so SetParent will work
        QT_WA({ SetWindowLong((HWND)winId(), GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS); },
              { SetWindowLongA((HWND)winId(), GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS); })
        QWindow *window = windowHandle();
        window->setProperty("_q_embedded_native_parent_handle", (WId)hParent);
        window->setFlags(Qt::FramelessWindowHint);
        HWND h = static_cast<HWND>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("handle", window));
        SetParent(h, hParent);
        // window->setFlags(Qt::FramelessWindowHint);
        QEvent e(QEvent::EmbeddingControl);
        QApplication::sendEvent(this, &e);
    }
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
HWND QWinWidget::parentWindow() const
{
    return hParent;
}

/*!
    \reimp
*/
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
        else if (e->removed() && reenable_parent)
        {
            reenable_parent = false;
            EnableWindow(hParent, true);
            obj->removeEventFilter(this);
        }
    }
    QWidget::childEvent(e);
}

/*! \internal */
void QWinWidget::saveFocus()
{
    if (!prevFocus)
        prevFocus = ::GetFocus();
    if (!prevFocus)
        prevFocus = parentWindow();
}

/*!
    Shows this widget. Overrides QWidget::show().

    \sa showCentered()
*/
void QWinWidget::show()
{
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
    GetWindowRect(hParent, &r);
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

/*!
    Sets the focus to the window that had the focus before this widget
    was shown, or if there was no previous window, sets the focus to
    the parent window.
*/
void QWinWidget::resetFocus()
{
    if (prevFocus)
        ::SetFocus(prevFocus);
    else
        ::SetFocus(parentWindow());
}

bool QWinWidget::nativeEvent(const QByteArray &, void *message, long *)
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

    return false;
}

/*!
    \reimp
*/
bool QWinWidget::eventFilter(QObject *o, QEvent *e)
{
    QWidget *w = (QWidget *)o;

    switch (e->type())
    {
    case QEvent::WindowDeactivate:
        if (w->isModal() && w->isHidden())
            BringWindowToTop(hParent);
        break;

    case QEvent::Hide:
        if (reenable_parent)
        {
            EnableWindow(hParent, true);
            reenable_parent = false;
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
            if (w->isModal() && !reenable_parent)
            {
                EnableWindow(hParent, false);
                reenable_parent = true;
            }
        }
        break;

    case QEvent::Close:
        ::SetActiveWindow(hParent);
        if (w->testAttribute(Qt::WA_DeleteOnClose))
            deleteLater();
        break;

    default:
        break;
    }

    return QWidget::eventFilter(o, e);
}

/*! \reimp
 */
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

/*! \reimp
 */
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
                else if (nextFocus->isWindow())
                {
                    topLevel = nextFocus;
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
        while (1)
        {
            nextFocus = nextFocus->nextInFocusChain();
            if (nextFocus->isWindow())
                break;
            if (nextFocus->focusPolicy() & Qt::TabFocus)
            {
                return QWidget::focusNextPrevChild(true);
            }
        }
    }

    ::SetFocus(hParent);

    return true;
}