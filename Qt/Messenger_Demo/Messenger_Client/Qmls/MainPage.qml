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

            ListView {
                id: chatRoomListView
                Layout.preferredWidth: mainPage.width / 4
                Layout.fillHeight: true
                clip: true

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
                
                model: ListModel {
                    id: charRoomListModel
                }
                
                delegate: Rectangle {
                    id: chatRoomRect
                    width: parent.width
                    height: 98
                    color: "#B240F5"

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            chatRoomRect.color = "#BD5CF5"
                        }
                        onExited: {
                            chatRoomRect.color = "#B240F5"
                        }
                    }
                }

                Component.onCompleted: {
                    for(var i=0;i < 15;i++) {
                        charRoomListModel.append({'chatRoomRect': "rect" + i})
                    }
                    // charRoomListModel.append({'chatRoomRect':one})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
                    // charRoomListModel.append({})
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