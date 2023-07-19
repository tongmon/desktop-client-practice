#include "widget.h"
#include "custommenubar.hpp"
#include "qwinwidget.h"

#include <QEvent>
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
    Pal.setColor(QPalette::Background, QColor(30, 34, 39));
    titleBar->setAutoFillBackground(true);
    titleBar->setPalette(Pal);

    // 타이틀바에 수평 레이아웃 추가
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleBar->setLayout(titleLayout);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(0);

    // 커스텀 메뉴 추가
    auto customMenu = new CustomMenuBar(titleBar);
    titleLayout->addWidget(customMenu);

    auto menu = customMenu->addMenu("File", {35, titleBar->height()});
    menu->addAction("New Text File");
    menu->addAction("New File");
    menu->addAction("New Window");
    menu->addAction("Open File");
    menu->addAction("Open Directory");

    menu = customMenu->addMenu("Edit", {35, titleBar->height()});
    menu->addAction("Undo");
    menu->addAction("Redo");
    menu->addAction("Cut");
    menu->addAction("Copy");
    menu->addAction("Paste");

    menu = customMenu->addMenu("Selected Zone", {90, titleBar->height()});
    menu->addAction("Select All");
    menu->addAction("Expand Select Zone");
    menu->addAction("Collapse Select Zone");
    menu->addAction("Add Cursor");
    menu->addAction("Select Line Mode");

    menu = customMenu->addMenu("View", {35, titleBar->height()});
    menu->addAction("Command Pallete");
    menu->addAction("Open View");
    menu->addAction("Shape");
    menu->addAction("Edit Layout");

    // 최소화 버튼
    minimizeButton = new QPushButton;
    minimizeButton->setFixedSize(35, 35);
    minimizeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    minimizeButton->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Minimize.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }

        QPushButton:hover {
            border-image: url(:/icon/MinimizeHover.png);
            background-color: rgba(255, 255, 255, 30%);
            background-repeat: no-repeat;
        }

        QPushButton:pressed {
            border-image: url(:/icon/Minimize.png);
            background-color: rgba(255, 255, 255, 60%);
            background-repeat: no-repeat;
        }
    )");
    titleLayout->addWidget(minimizeButton);

    // 최대화 버튼
    maximizeButton = new QPushButton;
    maximizeButton->setFixedSize(35, 35);
    maximizeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    maximizeButton->setCheckable(true);
    maximizeButton->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Maximize.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }

        QPushButton:hover {
            border-image: url(:/icon/MaximizeHover.png);
            background-color: rgba(255, 255, 255, 30%);
            background-repeat: no-repeat;
        }

        QPushButton:pressed {
            border-image: url(:/icon/Maximize.png);
            background-color: rgba(255, 255, 255, 60%);
            background-repeat: no-repeat;
        }

        QPushButton:checked {
            border-image: url(:/icon/Restore.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }

        QPushButton:checked:hover {
            border-image: url(:/icon/RestoreHover.png);
            background-color: rgba(255, 255, 255, 30%);
            background-repeat: no-repeat;
        }

        QPushButton:checked:pressed {
            border-image: url(:/icon/Restore.png);
            background-color: rgba(255, 255, 255, 60%);
            background-repeat: no-repeat;
        }
    )");
    titleLayout->addWidget(maximizeButton);

    // 닫기 버튼
    closeButton = new QPushButton;
    closeButton->setFixedSize(35, 35);
    closeButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    closeButton->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/icon/Close.png);
            background-color: rgba(255, 255, 255, 0%);
            background-repeat: no-repeat;
        }

        QPushButton:hover {
            border-image: url(:/icon/CloseHover.png);
            background-color: rgba(255, 255, 255, 30%);
            background-repeat: no-repeat;
        }

        QPushButton:pressed {
            border-image: url(:/icon/Close.png);
            background-color: rgba(255, 255, 255, 60%);
            background-repeat: no-repeat;
        }
    )");
    titleLayout->addWidget(closeButton);

    entireLayout->setAlignment(titleBar, Qt::AlignTop);

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
    // 타이틀바 필수 버튼 영역에 커서가 위치한지 검사
    if (minimizeButton->rect().contains(minimizeButton->mapFromGlobal(QCursor::pos())) ||
        maximizeButton->rect().contains(maximizeButton->mapFromGlobal(QCursor::pos())) ||
        closeButton->rect().contains(closeButton->mapFromGlobal(QCursor::pos())))
        return true;

    // 커스텀 타이틀바 메뉴 영역에 커서가 위치한지 검사
    for (int i = 0; i < titleBar->layout()->count(); i++)
    {
        auto customMenubar = dynamic_cast<CustomMenuBar *>(titleBar->layout()->itemAt(i)->widget());
        if (customMenubar)
        {
            auto menuLayout = customMenubar->layout();

            bool isContains = false;
            for (int j = 0; j < menuLayout->count() && !isContains; j++)
                isContains |= menuLayout->itemAt(j)->widget()->rect().contains(menuLayout->itemAt(j)->widget()->mapFromGlobal(QCursor::pos()));

            if (isContains)
                return true;
            else
                break;
        }
    }

    return false;
}
