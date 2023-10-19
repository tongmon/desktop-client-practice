#ifndef HEADER__FILE__LOGINPAGECONTEXT
#define HEADER__FILE__LOGINPAGECONTEXT

#include <QObject>
#include <QQmlComponent>

class WinQuickWindow;

class LoginPageContext : public QObject
{
    Q_OBJECT

    WinQuickWindow *m_window;
    std::string m_user_id;
    std::string m_user_pw;

  public:
    LoginPageContext(WinQuickWindow *window = nullptr);
    ~LoginPageContext();

    std::string GetUserID();
    std::string GetUserPW();

    Q_INVOKABLE void tryLogin(const QString &id, const QString &pw);
};

#endif /* HEADER__FILE__LOGINPAGECONTEXT */
