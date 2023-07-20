#ifndef HEADER__FILE__CUSTOMTITLEBAR
#define HEADER__FILE__CUSTOMTITLEBAR

#include <QLayout>
#include <QPushButton>
#include <QWidget>

class CustomTitleBar : public QWidget
{
    QString m_maxBtnStyle;
    QString m_maxDeactivatedBtnStyle;
    QString m_minBtnStyle;
    QString m_minDeactivatedBtnStyle;
    QString m_closeBtnStyle;
    QString m_closeDeactivatedBtnStyle;
    QPushButton *m_maximizeButton = nullptr;
    QPushButton *m_minimizeButton = nullptr;
    QPushButton *m_closeButton = nullptr;

    QHBoxLayout *m_titlebarLayout;

  public:
    CustomTitleBar(QWidget *parent, int height);
    ~CustomTitleBar();

    bool isClickEventAllowedZone();
    bool event(QEvent *evt);
    void addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = {0});

    QPushButton *GetMinimizeBtn()
    {
        return m_minimizeButton;
    }
    QPushButton *GetCloseBtn()
    {
        return m_closeButton;
    }
    QPushButton *GetMaximizeBtn()
    {
        return m_maximizeButton;
    }
};

#endif /* HEADER__FILE__CUSTOMTITLEBAR */
