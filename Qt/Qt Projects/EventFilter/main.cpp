#include "EventFilter.hpp"
#include "MainWidget.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;

    auto evtFilter = new EventFilter(&w);

    // 이벤트 필터를 QApplication에 설치했다.
    // EventFilter에서 특정 이벤트를 감지하여 QApplication으로 전달되는 것을 방지할 수 있다.
    a.installEventFilter(evtFilter);

    // 이벤트 필터를 다시 지워줄 수도 있다.
    // a.removeEventFilter(evtFilter);

    w.show();

    // 이벤트 획득, 판별은 QApplication에서 하는 것이기에 MainWidget에 EventFilter를 붙여도 소용없다.
    return a.exec();
}
