#ifndef HEADER__FILE__WINQUICKWINDOW
#define HEADER__FILE__WINQUICKWINDOW

#include "TCPClient.hpp"

#include <QAbstractNativeEventFilter>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QScreen>
#include <atomic>
#include <memory>
#include <tuple>
#include <vector>

class WinQuickWindow : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

    QQmlApplicationEngine *m_engine;
    QQuickWindow *m_quick_window;
    HWND m_hwnd;
    int m_resize_border_width;
    std::vector<std::pair<std::string, std::unique_ptr<QObject>>> m_context_properties;
    std::shared_ptr<TCPClient> m_tcp_client;

  public:
    WinQuickWindow(QQmlApplicationEngine *engine = nullptr);
    ~WinQuickWindow();

    HWND GetHandle();
    TCPClient &GetNetworkHandle();
    QQuickWindow &GetQuickWindow();
    bool InitWindow(QQmlApplicationEngine &engine);
    void OnScreenChanged(QScreen *screen);

    // 서버 -> 클라이언트 데이터 수신을 위해 상시 Read하고 있어야 함.
    void StartHandling();

    bool eventFilter(QObject *obj, QEvent *evt);
    bool nativeEventFilter(const QByteArray &event_type, void *message, long *result);

    // Q_INVOKABLE 함수들은 qml에서 직접 사용해야 하기에 첫글자를 소문자로 함
    Q_INVOKABLE void onMinimizeButtonClicked();
    Q_INVOKABLE void onMaximizeButtonClicked();
    Q_INVOKABLE void onCloseButtonClicked();

    std::atomic_bool is_tcp_stopped;
};

#endif /* HEADER__FILE__WINQUICKWINDOW */
