#include "EventFilter.hpp"

bool EventFilter::eventFilter(QObject *watched, QEvent *evt)
{
    // 마우스 클릭 이벤트를 가로채는 필터
    if (evt->type() == QEvent::MouseButtonPress)
    {
        qDebug() << "Mouse Press Event Catched by Filter!";

        // 밑과 같이 true를 반환하면 watched 객체는 클릭 이벤트를 감지하지 못한다.
        return true;

        // false를 반환하면 watched 객체는 클릭 이벤트를 감지할 수 있다.
        // return false;
    }

    // 밑과 같이 eventFilter 함수를 전파한다면 watched 객체는 이벤트를 정상적으로 감지할 수 있다.
    return QObject::eventFilter(watched, evt);
}