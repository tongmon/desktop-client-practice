import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    flags: Qt.Window | Qt.FramelessWindowHint
    color: Qt.rgba(0,0,0,0)

    property int windowEdgeRadius: 10
    property variant lstVisibility: []

    Rectangle {
        id: windowTitleBar
        width: parent.width; height: 30
        color: "blue"
        radius: 10
        anchors.top: parent.top;

        Text {
            id: titleText
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 5
            }
            color: "white"
            text: "This is Title Text"
        }

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

            // 윈도우 최대화 버튼
            Item {
                id: windowMaximizeButton
                height: windowTitleBar.height * 0.7
                width: height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    source: (mainWindow.visibility == Window.Maximized) ?
                        (maximizeBtnArea.containsMouse ? (maximizeBtnArea.pressed ? "qrc:/window_middlemize_pressed.png" : "qrc:/window_middlemize_hover.png") : "qrc:/window_middlemize.png") :
                        (maximizeBtnArea.containsMouse ? (maximizeBtnArea.pressed ? "qrc:/window_maximize_pressed.png" : "qrc:/window_maximize_hover.png") : "qrc:/window_maximize.png")
                    width: parent.width
                    height: parent.height

                    MouseArea {
                        id: maximizeBtnArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            mainWindow.visibility = (mainWindow.visibility == Window.Maximized) ? Window.Windowed : Window.Maximized
                        }

                        onHoveredChanged: {

                        }
                    }
                }
            }

            // 윈도우 최소화 버튼
            Item {
                id: windowMinimizeButton
                height: windowTitleBar.height * 0.7
                width: height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    source: minimizeBtnArea.containsMouse ? (minimizeBtnArea.pressed ? "qrc:/window_minimize_pressed.png" : "qrc:/window_minimize_hover.png") : "qrc:/window_minimize.png"
                    width: parent.width
                    height: parent.height

                    MouseArea {
                        id: minimizeBtnArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            mainWindow.visibility = Window.Minimized
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
    }

    onVisibilityChanged: {
        lstVisibility.push(visibility)
        if(lstVisibility.length > 3)
            lstVisibility.shift()

        // 윈도우 최대화시 모서리 둥근것 없애는 로직
        windowMainScreen.radius = windowTitleBar.radius = visibility == Window.Maximized ? 0 : windowEdgeRadius

        // 윈도우 최소화시 전의 창크기 유지하기 위한 로직
        if(visibility == Window.Maximized || visibility == Window.Windowed) {
            if(lstVisibility[1] === Window.Minimized)
                lstVisibility[0] === Window.Maximized ? showMaximized() : showNormal()
        }
    }
}

