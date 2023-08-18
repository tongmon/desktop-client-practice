#ifndef HEADER__FILE__LOGINPAGECONTEXT
#define HEADER__FILE__LOGINPAGECONTEXT

#include <QObject>

class LoginPageContext : public QObject
{
    Q_OBJECT

  public:
    LoginPageContext();
    ~LoginPageContext();

    Q_INVOKABLE bool tryLogin(const QString &id, const QString &pw);
};

#endif /* HEADER__FILE__LOGINPAGECONTEXT */
