#ifndef HEADER__FILE__WINQUICKWINDOW
#define HEADER__FILE__WINQUICKWINDOW

#include <QAbstractNativeEventFilter>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <Windows.h>
#include <Windowsx.h>
#include <memory>

class QmlConnectObj;
class WinNativeWindow;

class WinQuickWindow : public QAbstractNativeEventFilter
{
    friend class QmlConnectObj;

    QQuickWindow &m_window;
    HWND m_hwnd;
    std::unique_ptr<WinNativeWindow> m_parent_native_window;

    std::shared_ptr<QmlConnectObj> m_qml_connector;

  public:
    WinQuickWindow(QQuickWindow &quick_window, QQmlApplicationEngine &engine);
    ~WinQuickWindow();

    int border_width = 6;
    int titlebar_height = 35;

    void Show();
    void Center();
    void ShowCentered();
    void SetGeometry(int x, int y, int w, int h);
    void SetWidnowTitle(const QString &title);
    HWND GetParentHandle() const;
    HWND GetHandle() const;

  protected:
    bool IsTitleBarClickEventAllowedZone(const int &x, const int &y);
    bool nativeEventFilter(const QByteArray &event_type, void *message, long *result);
};

class QmlConnectObj : public QObject
{
    Q_OBJECT

    WinQuickWindow &m_quick_window;

  public:
    QmlConnectObj(WinQuickWindow &quick_window)
        : QObject(nullptr), m_quick_window{quick_window}
    {
    }

    ~QmlConnectObj()
    {
    }

    bool eventFilter(QObject *obj, QEvent *evt);

    Q_INVOKABLE void onMinimizeButtonClicked();

    Q_INVOKABLE void onMaximizeButtonClicked();

    Q_INVOKABLE void onCloseButtonClicked();
};

#endif /* HEADER__FILE__WINQUICKWINDOW */
