import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.2

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

        if (titleMenuBar.contains(titleMenuBar.mapFromGlobal(x, y)) === true)
            return true

        return false
    }

    // 부모창인 Native 윈도우의 활성 상태가 바뀔 때마다 호출됨
    function onParentNativeWindowStateChanged(parentActive) {
        console.log(parentActive)
    }

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: titleBar
            color: Qt.rgba(0.117, 0.133, 0.152, 1.0)
            width: parent.width
            height: titlebarHeight

            Row {
                spacing: 0
                anchors.fill: parent

                MenuBar {
                    id: titleMenuBar
                    anchors.left: parent.left

                    Menu {
                        title: qsTr("&File")
                        Action {
                            text: qsTr("&New...")
                        }
                        Action {
                            text: qsTr("&Open...")
                        }
                        Action {
                            text: qsTr("&Save")
                        }
                        Action {
                            text: qsTr("Save &As...")
                        }
                        MenuSeparator {}
                        Action {
                            text: qsTr("&Quit")
                        }
                    }
                    Menu {
                        title: qsTr("&Edit")
                        Action {
                            text: qsTr("Cu&t")
                        }
                        Action {
                            text: qsTr("&Copy")
                        }
                        Action {
                            text: qsTr("&Paste")
                        }
                    }
                    Menu {
                        title: qsTr("&Help")
                        Action {
                            text: qsTr("&About")
                        }
                    }

                    style: MenuBarStyle {}
                }

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
                                                 0.4) : (parent.hovered ? Qt.rgba(
                                                                              1.0, 1.0, 1.0,
                                                                              0.2) : Qt.rgba(
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
                                                 0.4) : (parent.hovered ? Qt.rgba(
                                                                              1.0, 1.0, 1.0,
                                                                              0.2) : Qt.rgba(
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
                                                 0.78, 0.16, 0.184,
                                                 0.6) : (parent.hovered ? Qt.rgba(
                                                                              0.86, 0.16, 0.184,
                                                                              0.9) : Qt.rgba(
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

        Rectangle {
            id: mainContent
            color: Qt.rgba(0.137, 0.152, 0.18, 1.0)
            width: parent.width
            height: parent.height - titlebarHeight
        }
    }
}
