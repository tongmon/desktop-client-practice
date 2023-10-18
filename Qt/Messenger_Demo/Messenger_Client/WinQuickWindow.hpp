#ifndef HEADER__FILE__WINQUICKWINDOW
#define HEADER__FILE__WINQUICKWINDOW

#include <QAbstractNativeEventFilter>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QScreen>
#include <atomic>
#include <memory>
#include <tuple>
#include <vector>

class TCPServer;
class TCPClient;

class WinQuickWindow : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

    QQmlApplicationEngine *m_engine;
    QQuickWindow *m_quick_window;
    HWND m_hwnd;
    int m_resize_border_width;
    std::vector<std::pair<std::string, std::unique_ptr<QObject>>> m_context_properties;

    std::shared_ptr<TCPClient> m_central_server;
    std::unique_ptr<TCPServer> m_local_server;

  public:
    WinQuickWindow(QQmlApplicationEngine *engine = nullptr);
    ~WinQuickWindow();

    HWND GetHandle();
    TCPClient &GetServerHandle();
    QQuickWindow &GetQuickWindow();
    bool InitWindow(QQmlApplicationEngine &engine);
    void OnScreenChanged(QScreen *screen);

    bool eventFilter(QObject *obj, QEvent *evt);
    bool nativeEventFilter(const QByteArray &event_type, void *message, long *result);

    // Q_INVOKABLE 함수들은 qml에서 직접 사용해야 하기에 첫글자를 소문자로 함
    Q_INVOKABLE void onMinimizeButtonClicked();
    Q_INVOKABLE void onMaximizeButtonClicked();
    Q_INVOKABLE void onCloseButtonClicked();
};

#endif /* HEADER__FILE__WINQUICKWINDOW */
