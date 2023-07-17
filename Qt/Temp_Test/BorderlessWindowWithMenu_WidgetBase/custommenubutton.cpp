#include "custommenubutton.hpp"

CustomMenuButton::CustomMenuButton(const QString &text, QWidget *parent, std::unordered_map<std::string, CustomMenuButton *> &others)
    : QPushButton(text, parent), m_others{others}
{
}

CustomMenuButton::~CustomMenuButton()
{
}

// void CustomMenuButton::enterEvent(QEvent *event)
// {
//     for (auto &item : m_others)
//     {
//         if (item.second != this && !item.second->menu()->isHidden())
//         {
//             item.second->menu()->hide();
//             // emit click();
//             showMenu();
//             return;
//         }
//     }
//
//     QPushButton::enterEvent(event);
// }
//
// void CustomMenuButton::leaveEvent(QEvent *event)
// {
//     QPushButton::leaveEvent(event);
// }
