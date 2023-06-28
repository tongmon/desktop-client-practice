#include "CustomButton.hpp"
#include <QDebug>

// event 함수를 재정의하면 해당 위젯에 가해지는 어떤 이벤트던 필터링 가능
bool CustomButton::event(QEvent *evt)
{
    // 더블클릭한 경우 메시지 띄우도록 변경
    if (evt->type() == QEvent::MouseButtonDblClick)
        qDebug() << "You are double clicked this button!";

    // 밑 부분을 해줘야 나머지 이벤트들이 정상 작동함.
    // 반대로 밑 부분을 비활성화해서 특정 이벤트만 작동하게 만들 수도 있음.
    return QPushButton::event(evt);
}

// 밑 처럼 특정 이벤트가 발생할 때만 수행되는 함수를 재정의하여 특정 이벤트를 필터링 할 수도 있음.
// leaveEvent는 QWidget에 존재하는 virtual 함수로 관련된 위젯에서 마우스가 벗어나는 경우 발생됨
void CustomButton::leaveEvent(QEvent *evt)
{
    qDebug() << "You are leaved this button!";

    return QPushButton::leaveEvent(evt);
}

// enterEvent는 QWidget에 존재하는 virtual 함수로 관련된 위젯에 마우스가 들어오는 경우 발생됨
void CustomButton::enterEvent(QEvent *evt)
{
    qDebug() << "You are entered this button!";

    return QPushButton::enterEvent(evt);
}
