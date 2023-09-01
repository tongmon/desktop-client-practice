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
            spacing: 0

            ListView {
                id: chatRoomListView
                Layout.preferredWidth: 250
                Layout.fillHeight: true
                clip: true

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
                
                model: ListModel {
                    id: chatRoomListModel
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
                        chatRoomListModel.append({'chatRoomRect': "rect" + i})
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60

                    RowLayout {
                        anchors.fill: parent
                        spacing: 0
                            
                        Text {
                            text: "Some Name"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        Button {
                            text: "Search"
                        }
                        Button {
                            text: "Chat Room Menu"
                        }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    model: ListModel {
                        id: chatListModel
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
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60

                    TextInput {
                        anchors.fill: parent
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60

                    RowLayout {
                        anchors.fill: parent
                        spacing: 0

                        Button {
                            Layout.fillHeight: true
                            text: "Imoji"
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        
    }
}