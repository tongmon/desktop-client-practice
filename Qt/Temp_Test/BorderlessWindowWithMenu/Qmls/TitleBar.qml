import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle {
    visible: true
    color: "beige"

    Column {
        Row {
            MenuBar {
                id: menu_bar
                height: 100
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
                        text: qsTr("&Cut")
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
            }

            Rectangle {
                width: 50
                height: 35
                color: "blue"
            }
        }
    }
}
