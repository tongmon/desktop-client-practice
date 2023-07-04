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

    property string myName: "tongstar"
    onMyNameChanged: {
        alert("My Name is changed to : " + myName)
        // console.log("My Name is changed to : " + myName)
    }
    
    Rectangle{
        width: 300
        height: 100
        color: "green"
        anchors.centerIn: parent

        MouseArea{          
            anchors.fill: parent
            onClicked: {
                myName = "kyungjoonlee"
            }
        }
    }
}