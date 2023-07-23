import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    visible: true
    minimumWidth: 200
    minimumHeight: 100
    flags: Qt.Window | Qt.FramelessWindowHint
    title: "CMake and Qt Quick"

    // 타이틀바 높이, C++에서 초기화해줌
    property int titlebarHeight: 0

    // C++에서 메시지를 받을 영역인지 구분하는 함수
    function isTitleBarClickEventAllowedZone(x, y) {
        var isContainMouse = minimumButton.contains(
                    minimumButton.mapFromGlobal(x, y))
        isContainMouse |= maximumButton.contains(maximumButton.mapFromGlobal(x,
                                                                             y))
        isContainMouse |= closeButton.contains(closeButton.mapFromGlobal(x, y))
        return isContainMouse
    }

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: title_bar
            color: "grey"
            width: parent.width
            height: titlebarHeight

            Row {
                spacing: 0
                anchors.fill: parent

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
}
