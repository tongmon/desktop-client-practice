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
                    id: minimumButton
                    width: 46
                    height: parent.height
                    anchors {
                        right: maximumButton.left
                    }
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(
                                                 1.0, 1.0, 1.0,
                                                 0.7) : (parent.hovered ? Qt.rgba(
                                                                              1.0, 1.0, 1.0,
                                                                              0.4) : Qt.rgba(
                                                                              1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: "qrc:/icon/Minimize.png"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }
                    onClicked: {

                        // console.info("image clicked!")
                    }
                }
                Button {
                    id: maximumButton
                    width: 46
                    height: parent.height
                    anchors {
                        right: closeButton.left
                    }
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(
                                                 1.0, 1.0, 1.0,
                                                 0.7) : (parent.hovered ? Qt.rgba(
                                                                              1.0, 1.0, 1.0,
                                                                              0.4) : Qt.rgba(
                                                                              1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: "qrc:/icon/Maximize.png"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }
                    onClicked: {

                        // console.info("image clicked!")
                    }
                }
                Button {
                    id: closeButton
                    width: 46
                    height: parent.height
                    anchors {
                        right: parent.right
                    }
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(
                                                 1.0, 1.0, 1.0,
                                                 0.7) : (parent.hovered ? Qt.rgba(
                                                                              1.0, 1.0, 1.0,
                                                                              0.4) : Qt.rgba(
                                                                              1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: "qrc:/icon/Close.png"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }
                    onClicked: {

                        // console.info("image clicked!")
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
