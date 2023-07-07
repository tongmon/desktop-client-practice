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
        width: my_text.implicitWidth + 20;
        height: my_text.implicitHeight + 20;
        color: "beige"
        border {
            color: "grey"
            width: 3
        }
        Text {
            id: my_text
            text: "My Text"   
            anchors.centerIn: parent
            font.pointSize: 30
        }

    }
}
