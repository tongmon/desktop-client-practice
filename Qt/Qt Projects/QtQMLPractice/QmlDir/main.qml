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

    property string myString: "My String"
    property int myInt: 1
    property bool myBool: true
    property var myPoint: Qt.point(10,10)
    property var myColor: Qt.rgba(1,1,1,0)

    Rectangle {
        width: 200
        height: 100
        anchors.centerIn: parent
        color: "yellow"
        
        Text {
            id: mTextId
            anchors.centerIn: parent
            text: myString
        }
    }
}