#ifndef HEADER__FILE__LOGINPAGECONTEXT
#define HEADER__FILE__LOGINPAGECONTEXT

#include <QObject>

class QQuickWindow;

class LoginPageContext : public QObject
{
    Q_OBJECT

    QQuickWindow *m_quick_window;

  public:
    LoginPageContext(QQuickWindow *quick_window = nullptr);
    ~LoginPageContext();

    Q_INVOKABLE void tryLogin(const QString &id, const QString &pw);
};

#endif /* HEADER__FILE__LOGINPAGECONTEXT */
