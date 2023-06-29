#include "ButtonEventFilter.hpp"
#include <QPushButton>

// 마우스 커서가 버튼 내부에 위치하면 버튼 텍스트를 바꾼다.
bool ButtonEventFilter::eventFilter(QObject *watched, QEvent *evt)
{
    if (auto btn = dynamic_cast<QPushButton *>(watched))
    {
        if (evt->type() == QEvent::HoverEnter)
        {
            btn->setText("In");
            return true;
        }
        else if (evt->type() == QEvent::HoverLeave)
        {
            btn->setText("Out");
            return true;
        }
    }

    return QObject::eventFilter(watched, evt);
}