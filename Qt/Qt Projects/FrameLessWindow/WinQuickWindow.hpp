#ifndef HEADER__FILE__WINQUICKWINDOW
#define HEADER__FILE__WINQUICKWINDOW

#include <QAbstractNativeEventFilter>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QScreen>
#include <Windows.h>
#include <Windowsx.h>
#include <dwmapi.h>
#include <memory>
#include <stdexcept>

class WinQuickWindow : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

    friend class NativeFilter;

  public:
    WinQuickWindow(QQuickWindow *quick_window = nullptr);
    ~WinQuickWindow();

    HWND GetHandle();
    bool SetQuickWindow(QQuickWindow *quick_window);
    void OnScreenChanged(QScreen *screen);

    bool eventFilter(QObject *obj, QEvent *evt);
    bool nativeEventFilter(const QByteArray &event_type, void *message, long *result);

    // Q_INVOKABLE 함수들은 qml에서 직접 사용해야 하기에 첫글자를 소문자로 함
    Q_INVOKABLE void onMinimizeButtonClicked();
    Q_INVOKABLE void onMaximizeButtonClicked();
    Q_INVOKABLE void onCloseButtonClicked();

  private:
    QQuickWindow *m_quick_window;
    HWND m_hwnd;
    int m_resize_border_width;
};

#endif /* HEADER__FILE__WINQUICKWINDOW */
