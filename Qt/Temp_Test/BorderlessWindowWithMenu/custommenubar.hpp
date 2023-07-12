#ifndef HEADER__FILE__CUSTOMMENUBAR
#define HEADER__FILE__CUSTOMMENUBAR

#include <QMenu>
#include <QMenuBar>
#include <string>
#include <unordered_map>

class CustomMenuBar : public QMenuBar
{
    std::unordered_map<std::string, QMenu *> m_menus;
    QWidget *m_menuConerWidget;

  public:
    CustomMenuBar(QWidget *parent = nullptr);
    ~CustomMenuBar();

    void addConerWidget(QWidget *widget);

    void paintEvent(QPaintEvent *event);
};

#endif /* HEADER__FILE__CUSTOMMENUBAR */
