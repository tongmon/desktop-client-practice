import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: mainWindow
    width: msgText.width + 20
    height: Math.max(msgText.height + 20, 200)
    visible: true
    title: qsTr("Hello World")
    flags: Qt.Window | Qt.FramelessWindowHint
    color: Qt.rgba(0,0,0,0)

    property int windowEdgeRadius: 10
    property string showMsg: "This is Message for Alert Box!"

    Rectangle {
        id: windowTitleBar
        width: parent.width; height: 30
        color: "blue"
        radius: 10
        anchors.top: parent.top;

        Rectangle {
            width: parent.radius
            height: parent.radius
            anchors {
                bottom: parent.bottom
                right: parent.right
            }
            color: parent.color
         }

        Rectangle {
            width: parent.radius
            height: parent.radius
            anchors {
                bottom: parent.bottom
                left: parent.left
            }
            color: parent.color
        }

        // 타이틀바 움직임
        DragHandler {
            id: titleResizeHandler
            onActiveChanged: {
                if (active)
                    mainWindow.startSystemMove()
            }
            target: null
        }

        Row {
            anchors.fill: parent
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            layoutDirection: Qt.RightToLeft
            spacing: 5

            // 윈도우 닫기 버튼
            Item {
                id: windowCloseButton
                height: windowTitleBar.height * 0.7
                width: height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    source: closeBtnArea.containsMouse ? (closeBtnArea.pressed ? "qrc:/window_close_pressed.png" : "qrc:/window_close_hover.png") : "qrc:/window_close.png"
                    width: parent.width
                    height: parent.height

                    MouseArea {
                        id: closeBtnArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            mainWindow.close();
                        }

                        onHoveredChanged: {

                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: windowMainScreen
        width: parent.width
        height: parent.height - windowTitleBar.height
        color: Qt.rgba(0.95,0.95,0.95,1)
        radius: 10
        anchors.top: windowTitleBar.bottom

        Rectangle {
            width: parent.radius
            height: parent.radius
            anchors {
                top: parent.top
                right: parent.right
            }
            color: parent.color
        }

        Rectangle {
            width: parent.radius
            height: parent.radius
            anchors {
                top: parent.top
                left: parent.left
            }
            color: parent.color
        }

        Text {
            id: msgText
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            text: showMsg
        }
    }
}

