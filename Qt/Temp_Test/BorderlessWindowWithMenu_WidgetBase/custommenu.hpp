#ifndef HEADER__FILE__CUSTOMMENU
#define HEADER__FILE__CUSTOMMENU

#include <QDebug>
#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include <string>
#include <unordered_map>

class CustomMenu : public QMenu
{
    std::unordered_map<std::string, QPushButton *> &m_menuButtons;

  public:
    CustomMenu(QWidget *parent, std::unordered_map<std::string, QPushButton *> &menuButtons);
    ~CustomMenu();

    void mouseMoveEvent(QMouseEvent *event);
};

#endif /* HEADER__FILE__CUSTOMMENU */
