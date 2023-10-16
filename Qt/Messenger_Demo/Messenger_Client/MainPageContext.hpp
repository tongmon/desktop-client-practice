#ifndef HEADER__FILE__MAINPAGECONTEXT
#define HEADER__FILE__MAINPAGECONTEXT

#include <QObject>
#include <QQmlComponent>

class WinQuickWindow;

class MainPageContext : public QObject
{
    Q_OBJECT

    WinQuickWindow *m_window;
    int m_recieve_id;

    int GetRequestID();

  public:
    MainPageContext(WinQuickWindow *window = nullptr);
    ~MainPageContext();

    Q_INVOKABLE void trySendTextChat(const QString &room_id, const QString &content);
    Q_INVOKABLE void initialChatRoomList(const QString &user_id);
};

#endif /* HEADER__FILE__MAINPAGECONTEXT */
