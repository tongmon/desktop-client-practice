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

    Rectangle {
        Row {
            Rectangle {
                width: 50
                height: menu_bar.height
                color: "blue"
            }

            MenuBar {
                id: menu_bar
                Menu {
                    title: qsTr("&File")
                    Action { text: qsTr("&New...") }
                    Action { text: qsTr("&Open...") }
                    Action { text: qsTr("&Save") }
                    Action { text: qsTr("Save &As...") }
                    MenuSeparator { }
                    Action { text: qsTr("&Quit") }
                }
                Menu {
                    title: qsTr("&Edit")
                    Action { text: qsTr("&Cut") }
                    Action { text: qsTr("&Copy") }
                    Action { text: qsTr("&Paste") }
                }
                Menu {
                    title: qsTr("&Help")
                    Action { text: qsTr("&About") }
                }
            }

            Rectangle {
                width: 50
                height: menu_bar.height
                color: "blue"
            }
        }
    }
}
