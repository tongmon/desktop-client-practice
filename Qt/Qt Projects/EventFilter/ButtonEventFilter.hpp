#ifndef HEADER__FILE__BUTTONEVENTFILTER
#define HEADER__FILE__BUTTONEVENTFILTER

#include <QDebug>
#include <QEvent>
#include <QObject>

class ButtonEventFilter : public QObject
{
    Q_OBJECT

  public:
    ButtonEventFilter(QObject *p = nullptr)
        : QObject(p)
    {
    }

    bool eventFilter(QObject *watched, QEvent *evt);
};

#endif /* HEADER__FILE__BUTTONEVENTFILTER */
