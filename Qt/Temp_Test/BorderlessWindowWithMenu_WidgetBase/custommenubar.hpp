#ifndef HEADER__FILE__CUSTOMMENUBAR
#define HEADER__FILE__CUSTOMMENUBAR

#include <QEnterEvent>
#include <QEvent>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <string>
#include <unordered_map>

class CustomMenuBar : public QWidget
{
    std::unordered_map<std::string, QPushButton *> m_menuButtons;
    QHBoxLayout *m_menuLayout;
    bool m_isEntered;
    bool m_menuPopedUp;

  public:
    CustomMenuBar(QWidget *parent = nullptr);
    ~CustomMenuBar();

    void paintEvent(QPaintEvent *event);
    // void enterEvent(QEnterEvent *event);
    // void leaveEvent(QEvent *event);
    // void mousePressEvent(QMouseEvent *event);
    // void mouseReleaseEvent(QMouseEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);
};

#endif /* HEADER__FILE__CUSTOMMENUBAR */
