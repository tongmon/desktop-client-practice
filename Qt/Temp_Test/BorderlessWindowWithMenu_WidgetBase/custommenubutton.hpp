#ifndef HEADER__FILE__CUSTOMMENUBUTTON
#define HEADER__FILE__CUSTOMMENUBUTTON

#include <QEnterEvent>
#include <QEvent>
#include <QMenu>
#include <QPushButton>
#include <unordered_map>

class CustomMenuButton : public QPushButton
{
    std::unordered_map<std::string, CustomMenuButton *> &m_others;

  public:
    CustomMenuButton(const QString &text, QWidget *parent, std::unordered_map<std::string, CustomMenuButton *> &others);
    ~CustomMenuButton();

    // void enterEvent(QEvent *event);
    // void leaveEvent(QEvent *event);
    // void mousePressEvent(QMouseEvent *event);
    // void mouseReleaseEvent(QMouseEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);
};

#endif /* HEADER__FILE__CUSTOMMENUBUTTON */
