#ifndef HEADER__FILE__QMAINPANEL
#define HEADER__FILE__QMAINPANEL

#include "QWinWidget.hpp"
#include <QMouseEvent>

class QMainPanel : public QWinWidget
{
    Q_OBJECT

  public:
    QMainPanel(HWND hWnd);
    bool nativeEvent(const QByteArray &, void *msg, long *result);
    void mousePressEvent(QMouseEvent *event);

  public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();

  private:
    HWND windowHandle;

  protected:
};

#endif /* HEADER__FILE__QMAINPANEL */
