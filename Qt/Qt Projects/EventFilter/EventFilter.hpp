#ifndef HEADER__FILE__EVENTFILTER
#define HEADER__FILE__EVENTFILTER

#include <QDebug>
#include <QEvent>
#include <QObject>

class EventFilter : public QObject
{
    Q_OBJECT

  public:
    EventFilter(QObject *p = nullptr)
        : QObject(p)
    {
    }

    bool eventFilter(QObject *watched, QEvent *evt);
};

#endif /* HEADER__FILE__EVENTFILTER */
