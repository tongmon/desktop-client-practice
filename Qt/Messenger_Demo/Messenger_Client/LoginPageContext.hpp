#ifndef HEADER__FILE__LOGINPAGECONTEXT
#define HEADER__FILE__LOGINPAGECONTEXT

#include <QObject>
#include <QQmlComponent>
#include <memory>

class WinQuickWindow;

class LoginPageContext : public QObject
{
    Q_OBJECT

    WinQuickWindow *m_window;

  public:
    LoginPageContext(WinQuickWindow *window = nullptr);
    ~LoginPageContext();

    Q_INVOKABLE void tryLogin(const QString &id, const QString &pw);
};

#endif /* HEADER__FILE__LOGINPAGECONTEXT */
