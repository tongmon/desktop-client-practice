#ifndef HEADER__FILE__CUSTOMMENUBAR
#define HEADER__FILE__CUSTOMMENUBAR

#include <QEnterEvent>
#include <QEvent>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTime>
#include <string>
#include <tuple>
#include <unordered_map>

class CustomMenuBar : public QWidget
{
    std::unordered_map<std::string, QPushButton *> m_menuButtons;
    QHBoxLayout *m_menuLayout;
    QString m_casualStyle;
    QString m_hoverStyle;
    QString m_deactivatedStyle;
    bool m_isWin8OrGreater;

  public:
    CustomMenuBar(QWidget *parent = nullptr);
    ~CustomMenuBar();

    bool isClickEventAllowedZone();
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    bool event(QEvent *evt);
    QMenu *addMenu(const QString &menu, const QSize &size = QSize(35, 35));
};

#endif /* HEADER__FILE__CUSTOMMENUBAR */
