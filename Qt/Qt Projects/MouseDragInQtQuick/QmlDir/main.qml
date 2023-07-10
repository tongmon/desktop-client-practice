import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    visible: true // 윈도우 보임 여부
    width: 500 // 윈도우 초기 너비   
    height: 300 // 윈도우 초기 높이
    minimumWidth: 200 // 윈도우 최소 너비
    minimumHeight: 100 // 윈도우 최대 너비
    title: "CMake and Qt Quick" // 윈도우 창 제목

    Rectangle{
        id: container_rect
        width: parent.width
        height: parent.height
        color: "beige"

        // 마우스 상호작용 가능 사각형
        Rectangle {
            id: draggable_rect
            width: 50
            height: 50
            color: "blue"

            MouseArea {
                onClicked: {

                }

                onWheel: {

                }

                hoverEnabled: true
                onHoveredChanged: {
                    draggable_rect.color = containsMouse ? "red" : "blue"      
                }

                // 마우스 드래그를 위한 영역 지정
                anchors.fill: parent
                drag.target: draggable_rect
                drag.axis: Drag.XAndYAxis
                drag.minimumX: 0
                drag.maximumX: container_rect.width - draggable_rect.width
                drag.minimumY: 0
                drag.maximumY: container_rect.height - draggable_rect.height
            }
        }
    }
}
