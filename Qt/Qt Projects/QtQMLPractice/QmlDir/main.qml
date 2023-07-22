import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    id: mainWindow
    // width: 640
    // height: 480
    visible: true
    title: qsTr("Hello World")
    flags: Qt.Window | Qt.FramelessWindowHint
    color: Qt.rgba(0,0,0,0)

    Rectangle {
        id: windowTitleBar
        width: parent.width; height: parent.height
        color: "blue"
    }
}