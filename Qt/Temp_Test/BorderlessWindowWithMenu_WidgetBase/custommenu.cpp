#include "custommenu.hpp"
#include <QDebug>

CustomMenu::CustomMenu(QWidget *parent, std::unordered_map<std::string, QPushButton *> &menuButtons)
    : QMenu(parent), m_menuButtons{menuButtons}
{
}

CustomMenu::~CustomMenu()
{
}

void CustomMenu::mouseMoveEvent(QMouseEvent *event)
{
    for (auto &item : m_menuButtons)
    {
        // if (item.second->rect().contains(event->pos()) && item.second != reinterpret_cast<QPushButton *>(parent()))
        // {
        //     reinterpret_cast<QPushButton *>(parent())->menu()->hide();
        //     item.second->menu()->show();
        //     return;
        // }

        //     if (item.second->menu() == this)
        //         continue;
        //
        // if (item.second->rect().contains(event->pos()))
        //     item.second->showMenu();
        // else
        //     item.second->menu()->hide();
    }
    QMenu::mouseMoveEvent(event);
}
