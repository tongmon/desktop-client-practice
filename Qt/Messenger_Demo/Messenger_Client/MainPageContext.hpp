#ifndef HEADER__FILE__MAINPAGECONTEXT
#define HEADER__FILE__MAINPAGECONTEXT

#include <QObject>
#include <QQmlComponent>

class WinQuickWindow;

class MainPageContext : public QObject
{
    Q_OBJECT

    WinQuickWindow *m_window;

  public:
    MainPageContext(WinQuickWindow *window = nullptr);
    ~MainPageContext();

    Q_INVOKABLE void trySendTextChat(const QString &room_id, const QString &content);
};

#endif /* HEADER__FILE__MAINPAGECONTEXT */
