#include "custommenubar.hpp"
#include "widget.h"
#include <QEvent>
#include <QFont>
#include <QLayout>

CustomMenuBar::CustomMenuBar(QWidget *parent)
    : QMenuBar{parent}
{
    setStyleSheet(R"(
        QMenuBar {
            background-color: rgba(255, 255, 255, 0%);
            /* color: rgba(180, 180, 180, 100%); */
            /* background-repeat: no-repeat; */
            /* padding-left: 35px; */
        }
 
        QMenuBar::item { 
            /*padding-top: 5px;*/
            font-size: 100px;
        }
    )");

    m_menus["File"] = addMenu("&File");
    m_menus["File"]->addAction("New Text File");
    m_menus["File"]->addAction("New File");
    m_menus["File"]->addAction("New Window");
    m_menus["File"]->addAction("Open File");
    m_menus["File"]->addAction("Open Directory");

    m_menus["Edit"] = addMenu("&Edit");
    m_menus["Edit"]->addAction("Undo");
    m_menus["Edit"]->addAction("Redo");
    m_menus["Edit"]->addAction("Cut");
    m_menus["Edit"]->addAction("Copy");
    m_menus["Edit"]->addAction("Paste");

    m_menus["View"] = addMenu("&View");
    m_menus["View"]->addAction("Open View");

    auto shapeMenu = m_menus["Edit"]->addMenu("Shape");
    shapeMenu->addAction("Full Screen Mode");
    shapeMenu->addAction("Zen Mode");
    shapeMenu->addAction("Mini Map Mode");

    m_menus["View"]->addAction("Search");
    m_menus["View"]->addAction("Execute");

    // 타이틀바 버튼 추가를 위한 위젯 생성
    m_menuConerWidget = new QWidget(this);
    m_menuConerWidget->setContentsMargins(0, 0, 0, 0);

    // 타이틀바 버튼이 추가될 레이아웃
    QHBoxLayout *menuButtonLayout = new QHBoxLayout(m_menuConerWidget);
    m_menuConerWidget->setLayout(menuButtonLayout);
    menuButtonLayout->setContentsMargins(0, 0, 0, 0);
    menuButtonLayout->setSpacing(0);

    setCornerWidget(m_menuConerWidget, Qt::Corner::TopRightCorner);
}

CustomMenuBar::~CustomMenuBar()
{
}

void CustomMenuBar::addConerWidget(QWidget *widget)
{
    m_menuConerWidget->layout()->addWidget(widget);
}

void CustomMenuBar::paintEvent(QPaintEvent *event)
{
    setFixedSize(reinterpret_cast<QWidget *>(parent())->size());

    QMenuBar::paintEvent(event);
}