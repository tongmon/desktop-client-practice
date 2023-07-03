#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>

class Widget : public QMainWindow
{
    Q_OBJECT
  public:
    explicit Widget(QWidget *parent = 0);

    // If you want to have Max/Min/Close buttons, look at how QWinWidget uses these
    QPushButton *maximizeButton = nullptr;
    QPushButton *minimizeButton = nullptr;
    QPushButton *closeButton = nullptr;

    // 타이틀 바
    QWidget *titleBar = nullptr;

    bool eventFilter(QObject *obj, QEvent *event);

  signals:

  public slots:
};

#endif // WIDGET_H