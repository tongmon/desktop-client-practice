#ifndef HEADER__FILE__WINQUICKWINDOW
#define HEADER__FILE__WINQUICKWINDOW

#include <QAbstractNativeEventFilter>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <Windows.h>
#include <Windowsx.h>

class WinNativeWindow;

class WinQuickWindow : public QAbstractNativeEventFilter
{
    QQuickWindow &m_window;
    WinNativeWindow *m_parent_native_window;

  public:
    WinQuickWindow(QQuickWindow &quick_window);
    ~WinQuickWindow();

    int border_width = 6;
    int titlebar_height = 35;

    void Show();
    void Center();
    void ShowCentered();
    void SetGeometry(int x, int y, int w, int h);
    void SetWidnowTitle(const QString &title);
    HWND GetParentHandle() const;

  protected:
    bool nativeEventFilter(const QByteArray &event_type, void *message, long *result);
};

#endif /* HEADER__FILE__WINQUICKWINDOW */
