import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: mainPage
    color: "#280a3d"
    objectName: "mainPage"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TitleBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 35
            Layout.alignment: Qt.AlignTop

            Component.onCompleted: {
                addItem(String.raw`
                        Button {
                            height: parent.height
                            text: "Setting"
                        }
                        `)

                addItem(String.raw`
                        Button {
                            height: parent.height
                            text: "Contact"
                        }
                        `)

                addItem(String.raw`
                        Button {
                            height: parent.height
                            text: "Chat"
                        }
                        `)
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // 리스트뷰를 마우스 휠로 조작하려면 밑 링크 참조
            // https://stackoverflow.com/questions/75505046/qml-how-to-scroll-scrollview-using-the-mouse-wheel
            ListView {
                Layout.preferredWidth: mainPage.width / 4
                Layout.fillHeight: true
                clip: true
                
                model: ListModel {
                    id: charRoomListModel
                }
                
                delegate: Rectangle {
                    width: parent.width
                    height: 50
                    color: "white"
                }

                Component.onCompleted: {
                    charRoomListModel.append({})
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Component.onCompleted: {
        
    }
}