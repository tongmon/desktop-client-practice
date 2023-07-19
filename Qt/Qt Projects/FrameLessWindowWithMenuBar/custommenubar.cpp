#include "custommenubar.hpp"
#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QFont>
#include <QLayout>
#include <QStyleOptionButton>
#include <QStylePainter>

CustomMenuBar::CustomMenuBar(QWidget *parent)
    : QWidget{parent}
{
    m_menuLayout = new QHBoxLayout(parent);
    setLayout(m_menuLayout);
    m_menuLayout->setContentsMargins(0, 0, 0, 0);
    m_menuLayout->setSpacing(0);
    m_menuLayout->setAlignment(Qt::AlignLeft);

    m_casualStyle = R"(
                        QPushButton {
                            border-image: url(:/icon/Transparent.png);
                            color: rgb(152, 160, 175);
                            background-color: rgba(255, 255, 255, 0%);
                            background-repeat: no-repeat;
                        }
                        QPushButton:hover {
                            background-color: rgba(255, 255, 255, 30%);
                            background-repeat: no-repeat;
                        }
                        QPushButton:pressed {
                            background-color: rgba(255, 255, 255, 30%);
                            background-repeat: no-repeat;
                        }
                        QPushButton::menu-indicator {
                            width: 0px;
                        }
                    )";

    m_hoverStyle = R"(
                        QPushButton {
                            border-image: url(:/icon/Transparent.png);
                            color: rgb(152, 160, 175);
                            background-color: rgba(255, 255, 255, 30%);
                            background-repeat: no-repeat;
                        }
                        QPushButton::menu-indicator {
                            width: 0px;
                        }
                    )";
}

CustomMenuBar::~CustomMenuBar()
{
}

void CustomMenuBar::paintEvent(QPaintEvent *event)
{
    // setFixedSize(reinterpret_cast<QWidget *>(parent())->size());

    QWidget::paintEvent(event);
}

bool CustomMenuBar::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick)
        return true;

    if (event->type() == QEvent::MouseButtonPress)
        return true;

    if (event->type() == QEvent::MouseButtonRelease)
    {
        QPushButton *btn = dynamic_cast<QPushButton *>(obj);
        if (btn)
        {
            btn->menu()->popup(btn->mapToGlobal({btn->rect().left(),
                                                 btn->rect().top() + btn->height()}));
        }
        else
        {
            QPushButton *btn = nullptr;
            for (auto &item : m_menuButtons)
            {
                if (item.second->menu()->isVisible())
                {
                    btn = item.second;
                    break;
                }
            }

            btn->setStyleSheet(m_casualStyle);
            btn->menu()->hide();
            btn->click();
        }

        // Windows 7 이하에서는 해줘야 됨
        // repaint();

        return true;
    }

    if (event->type() == QEvent::MouseMove)
    {
        bool isContainedChanged = false;
        static QPushButton *containedBtn = nullptr;
        QPushButton *menuVisibleBtn = nullptr;
        for (auto &item : m_menuButtons)
        {
            if (item.second->menu()->isVisible())
                menuVisibleBtn = item.second;
            if (item.second->rect().contains(item.second->mapFromGlobal(QCursor::pos())) && item.second != containedBtn)
            {
                isContainedChanged = true;
                containedBtn = item.second;
            }
        }

        // 켜져있는 메뉴가 하나라도 없으면 마우스 움직이는 이벤트 그대로 전달
        if (!menuVisibleBtn || !isContainedChanged)
            return QObject::eventFilter(obj, event);

        if (containedBtn->menu()->isHidden() && containedBtn->rect().contains(containedBtn->mapFromGlobal(QCursor::pos())))
        {
            menuVisibleBtn->setStyleSheet(m_casualStyle);
            menuVisibleBtn->menu()->hide();
            menuVisibleBtn->click();

            containedBtn->setStyleSheet(m_hoverStyle);
            containedBtn->menu()->popup(containedBtn->mapToGlobal({containedBtn->rect().left(),
                                                                   containedBtn->rect().top() + containedBtn->height()}));
        }

        // Windows 7 이하에서는 해줘야 됨
        // repaint();

        return true;
    }

    return QObject::eventFilter(obj, event);
}

QMenu *CustomMenuBar::addMenu(const QString &menuTitle, const QSize &size)
{
    if (m_menuButtons.find(menuTitle.toStdString()) != m_menuButtons.end())
        return nullptr;

    auto stdTitle = menuTitle.toStdString();

    m_menuButtons[stdTitle] = new QPushButton(menuTitle, this);

    auto menu = new QMenu(m_menuButtons[menuTitle.toStdString()]);
    menu->installEventFilter(this);
    menu->setStyleSheet(R"(
         QMenu {
             color: rgb(152, 160, 175);
             background-color: rgb(30, 34, 39);
         }
         QMenu::item:selected {
             background-color: rgb(52, 56, 61);
         }
     )");

    m_menuButtons[stdTitle]->setMenu(menu);
    m_menuButtons[stdTitle]->installEventFilter(this);
    m_menuButtons[stdTitle]->setFixedSize(size);
    m_menuButtons[stdTitle]->setFocusPolicy(Qt::NoFocus);
    m_menuButtons[stdTitle]->setStyleSheet(m_casualStyle);

    m_menuLayout->addWidget(m_menuButtons[stdTitle]);

    return menu;
}