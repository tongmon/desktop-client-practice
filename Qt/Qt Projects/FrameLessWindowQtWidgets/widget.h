#ifndef WIDGET_H
#define WIDGET_H

#include "customtitlebar.hpp"

#include <QEvent>
#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>

class Widget : public QMainWindow
{
    Q_OBJECT

    // 타이틀 바
    CustomTitleBar *m_titleBar = nullptr;

  public:
    explicit Widget(QWidget *parent = 0);

    bool isClickEventAllowedZone();
    bool event(QEvent *evt);

    QPushButton *GetMinimizeBtn();
    QPushButton *GetCloseBtn();
    QPushButton *GetMaximizeBtn();

    QWidget *GetTitleBar();

  signals:

  public slots:
};

#endif // WIDGET_H