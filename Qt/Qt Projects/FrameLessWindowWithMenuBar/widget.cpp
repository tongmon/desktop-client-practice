#include "widget.h"
#include "custommenubar.hpp"
#include "qwinwidget.h"

#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QPixmap>

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
    m_titleBar = new CustomTitleBar(mainWidget, reinterpret_cast<QWinWidget *>(parent)->TOOLBARHEIGHT);
    entireLayout->addWidget(m_titleBar);

    // 타이틀바 색상 결정
    QPalette Pal(m_titleBar->palette());
    Pal.setColor(QPalette::Background, QColor(30, 34, 39));
    m_titleBar->setAutoFillBackground(true);
    m_titleBar->setPalette(Pal);

    // 타이틀바 아이콘 추가
    QLabel *titleBarIcon = new QLabel(m_titleBar);
    titleBarIcon->setPixmap(QPixmap(":/icon/ApplicationIcon.png").scaled(m_titleBar->height() * 0.7f, m_titleBar->height() * 0.7f, Qt::KeepAspectRatio));
    titleBarIcon->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    titleBarIcon->setFixedSize(m_titleBar->height(), m_titleBar->height());
    m_titleBar->addWidget(titleBarIcon);

    // 커스텀 메뉴 추가
    auto customMenu = new CustomMenuBar(m_titleBar);
    m_titleBar->addWidget(customMenu);

    auto menu = customMenu->addMenu("File", {35, m_titleBar->height()});
    menu->addAction("New Text File");
    menu->addAction("New File");
    menu->addAction("New Window");
    menu->addAction("Open File");
    menu->addAction("Open Directory");

    menu = customMenu->addMenu("Edit", {35, m_titleBar->height()});
    menu->addAction("Undo");
    menu->addAction("Redo");
    menu->addAction("Cut");
    menu->addAction("Copy");
    menu->addAction("Paste");

    menu = customMenu->addMenu("Selected Zone", {90, m_titleBar->height()});
    menu->addAction("Select All");
    menu->addAction("Expand Select Zone");
    menu->addAction("Collapse Select Zone");
    menu->addAction("Add Cursor");
    menu->addAction("Select Line Mode");

    menu = customMenu->addMenu("View", {35, m_titleBar->height()});
    menu->addAction("Command Pallete");
    menu->addAction("Open View");
    menu->addAction("Shape");
    menu->addAction("Edit Layout");

    entireLayout->setAlignment(m_titleBar, Qt::AlignTop);

    // 메인 윈도우 추가
    QWidget *mainwindowWidget = new QWidget(mainWidget);
    entireLayout->addWidget(mainwindowWidget);
    mainwindowWidget->setContentsMargins(0, 0, 0, 0);
    mainwindowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 메인 윈도우 색상 결정
    Pal = mainwindowWidget->palette();
    Pal.setColor(QPalette::Background, QColor(35, 39, 46));
    mainwindowWidget->setAutoFillBackground(true);
    mainwindowWidget->setPalette(Pal);

    // 메인 윈도우에 수직 레이아웃 추가
    QVBoxLayout *mainwindowLayout = new QVBoxLayout(mainwindowWidget);
    mainwindowWidget->setLayout(mainwindowLayout);
}

// 타이틀바에서 클릭이 허용된 부분인지 검사
bool Widget::isClickEventAllowedZone()
{
    return m_titleBar->isClickEventAllowedZone();
}

bool Widget::event(QEvent *evt)
{
    if (evt->type() == QEvent::WindowActivate || evt->type() == QEvent::WindowDeactivate)
    {
        QApplication::sendEvent(m_titleBar, evt);
        return true;
    }

    return QMainWindow::event(evt);
}

QPushButton *Widget::GetMinimizeBtn()
{
    return m_titleBar->GetMinimizeBtn();
}

QPushButton *Widget::GetCloseBtn()
{
    return m_titleBar->GetCloseBtn();
}

QPushButton *Widget::GetMaximizeBtn()
{
    return m_titleBar->GetMaximizeBtn();
}

QWidget *Widget::GetTitleBar()
{
    return m_titleBar;
}
