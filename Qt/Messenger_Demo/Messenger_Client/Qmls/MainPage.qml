import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    color: "#280a3d"

    Button {
        text: "This is Button!"
        anchors.centerIn: parent

        onClicked: {
            mainWindowLoader.source = "qrc:/qml/LoginPage.qml"
        }
    }
}