import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    visible: true
    minimumWidth: 200
    minimumHeight: 100
    flags: Qt.Window | Qt.FramelessWindowHint
    // title: "CMake and Qt Quick"

    // 타이틀바 높이, C++에서 초기화해줌
    property int titlebarHeight: 0

    // C++에서 메시지를 받을 영역인지 구분하는 함수
    function isTitleBarClickEventAllowedZone(x, y) {
        if (testButton.contains(testButton.mapFromGlobal(x, y)) === true)
            return true

        if (minimumButton.contains(minimumButton.mapFromGlobal(x, y)) === true)
            return true

        if (maximumButton.contains(maximumButton.mapFromGlobal(x, y)) === true)
            return true

        if (closeButton.contains(closeButton.mapFromGlobal(x, y)) === true)
            return true

        return false
    }

    function onParentNativeWindowStateChanged(parentActive) {
        console.log(parentActive)
    }

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: titleBar
            color: "grey"
            width: parent.width
            height: titlebarHeight

            Row {
                spacing: 0
                anchors.fill: parent

                Button {
                    id: testButton
                    text: "button"
                    height: parent.height
                    opacity: 0.6
                    anchors {
                        right: minimumButton.left
                    }
                    onHoveredChanged: {
                        hovered ? testButton.opacity = 1.0 : testButton.opacity = 0.6
                    }
                }

                Rectangle {
                    id: minimumButton
                    color: "pink"
                    width: 46
                    height: parent.height
                    anchors {
                        right: maximumButton.left
                    }
                }

                Rectangle {
                    id: maximumButton
                    color: "red"
                    width: 46
                    height: parent.height
                    anchors {
                        right: closeButton.left
                    }
                }

                Rectangle {
                    id: closeButton
                    color: "blue"
                    width: 46
                    height: parent.height
                    anchors {
                        right: parent.right
                    }
                }
            }
        }
    }

    onWindowStateChanged: {
        console.log(windowState)
    }

    //onActiveChanged: {
    //    console.log(parentActive)
    //}
}
