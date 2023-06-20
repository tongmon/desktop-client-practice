#include "widget.h"
#include "qwinwidget.h"

#include <QLabel>
#include <QLayout>

Widget::Widget(QWidget *parent)
    : QMainWindow(parent)
{
    // 메인 윈도우라 위젯을 먼저 추가함
    QWidget *mainWidget = new QWidget(this);
    mainWidget->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(mainWidget);

    // 메인 윈도우에 수직 레이아웃 추가
    QVBoxLayout *entireLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(entireLayout);
    entireLayout->setContentsMargins(0, 0, 0, 0);
    entireLayout->setSpacing(0);

    // 타이틀바 위젯 추가
    titleBar = new QWidget(mainWidget);
    entireLayout->addWidget(titleBar);
    titleBar->setContentsMargins(0, 0, 0, 0);
    titleBar->setFixedHeight(reinterpret_cast<QWinWidget *>(parent)->TOOLBARHEIGHT);

    // 타이틀바 색상 결정
    QPalette Pal(titleBar->palette());
    Pal.setColor(QPalette::Background, QColor(70, 69, 71));
    titleBar->setAutoFillBackground(true);
    titleBar->setPalette(Pal);

    // 타이틀바에 수평 레이아웃 추가
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleBar->setLayout(titleLayout);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(0);
    titleLayout->setAlignment(Qt::AlignRight);

    // 최소화 버튼
    minimizeButton = new QPushButton("-");
    minimizeButton->setFixedSize(30, 30);
    minimizeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    titleLayout->addWidget(minimizeButton);

    // 최대화 버튼
    maximizeButton = new QPushButton("+");
    maximizeButton->setFixedSize(30, 30);
    maximizeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    titleLayout->addWidget(maximizeButton);

    // 닫기 버튼
    closeButton = new QPushButton("X");
    closeButton->setFixedSize(30, 30);
    closeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    titleLayout->addWidget(closeButton);

    entireLayout->setAlignment(titleBar, Qt::AlignTop);

    // 메인 윈도우 추가
    QWidget *mainwindowWidget = new QWidget(mainWidget);
    entireLayout->addWidget(mainwindowWidget);
    mainwindowWidget->setContentsMargins(0, 0, 0, 0);
    mainwindowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 메인 윈도우 색상 결정
    Pal = mainwindowWidget->palette();
    Pal.setColor(QPalette::Background, QColor(175, 174, 176));
    mainwindowWidget->setAutoFillBackground(true);
    mainwindowWidget->setPalette(Pal);

    // 메인 윈도우에 수직 레이아웃 추가
    QVBoxLayout *mainwindowLayout = new QVBoxLayout(mainwindowWidget);
    mainwindowWidget->setLayout(mainwindowLayout);
}
