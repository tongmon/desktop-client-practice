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

    Row {
        Button {
            id: menu_btn_1
            text: "File"
            width: 45
            height: 35
            property bool m_clicked: false

            onClicked: {
                m_clicked = ~m_clicked
                if (m_clicked)
                    menu_btn_1_menu.open()
                else
                    menu_btn_1_menu.close()
            }

            Menu {
                id: menu_btn_1_menu
                y: menu_btn_1.height

                MenuItem {
                    text: "New..."
                }
                MenuItem {
                    text: "Open..."
                }
                MenuItem {
                    text: "Save"
                }
            }
        }

        Button {
            id: menu_btn_2
            text: "Settings"
            width: 45
            height: 35
            property bool m_clicked: false

            onPressed: {

                // menu_btn_2_menu.close()
            }

            onReleased: {

            }

            onClicked: {
                m_clicked = ~m_clicked
                if (m_clicked)
                    menu_btn_2_menu.open()
            }

            Menu {
                id: menu_btn_2_menu
                y: menu_btn_2.height

                MenuItem {
                    text: "New..."
                }
                MenuItem {
                    text: "Open..."
                }
                MenuItem {
                    text: "Save"
                }
            }
        }
    }
}
