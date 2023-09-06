import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import ChatBubbleEnum 1.0

Rectangle {
    id: mainPage
    color: "#280a3d"
    objectName: "mainPage"

    function addChatBox(isOpposite, contentType, content)
    {
        
    }

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

                    RowLayout {
                        anchors.fill: parent
                        spacing: 0
                            
                        Rectangle {
                            id: chatRoomImageRect
                            Layout.fillHeight: true
                            Layout.preferredWidth: chatRoomImageRect.height
                            color: chatRoomRect.color

                            Rectangle {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                height: parent.height * 0.8
                                width: parent.width * 0.8
                                radius: width * 2

                                Image {
                                    anchors.fill: parent
                                    source: "qrc:/icon/UserID.png" 
                                    fillMode: Image.PreserveAspectFit
                                }    
                            }
                        }

                        ColumnLayout {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            spacing: 0

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                Text {
                                    anchors.verticalCenter: parent.verticalCenter 
                                    anchors.left: parent.left
                                    text: chatRoomName
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                Text {                               
                                    anchors.verticalCenter: parent.verticalCenter 
                                    anchors.left: parent.left                            
                                    text: chatRoomPreview
                                }                            
                            }
                        }
                    }

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
                        chatRoomListModel.append({
                            "chatRoomRect": "rect" + i,
                            "chatRoomName": "Chat Room Num:" + i,
                            "chatRoomPreview": "Chat Room Preview"
                            })
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
                    id: chatBubbleListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 15

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    model: ListModel {
                        id: chatListModel
                    }

                    delegate: Rectangle {
                        color: "transparent"
                        width: parent.width
                        height: chatBubbleLoader.height

                        property size chatBubbleEdgeSize: Qt.size(20, 20)
                        property var chatBubbleMaximumWidth: chatBubbleListView.width * 0.6

                        RowLayout {       
                            anchors.fill: parent
                            spacing: 0
                            
                            Item {
                                Layout.fillWidth: isOpposite
                            }

                            Canvas {
                                Layout.preferredWidth: isOpposite ? 0 : chatBubbleEdgeSize.width
                                Layout.preferredHeight: isOpposite ? 0 : chatBubbleEdgeSize.height
                                Layout.alignment: (isOpposite ? Qt.AlignRight : Qt.AlignLeft) | Qt.AlignTop

                                onPaint: {
                                    var context = getContext("2d")

                                    context.beginPath()
                                    context.moveTo(0,0)
                                    context.lineTo(width,0)
                                    context.lineTo(width,height)
                                    context.lineTo(0,0)
                                    context.closePath()

                                    context.fillStyle = "blue"
                                    context.fill()
                                }
                            }
                            
                            Loader {          
                                id: chatBubbleLoader
                                Layout.preferredWidth: (item !== null && typeof(item) !== 'undefined') ? item.width : 0
                                Layout.preferredHeight: (item !== null && typeof(item) !== 'undefined') ? item.height : 0
                                Layout.alignment: Qt.AlignTop
                                source: chatBubbleSource

                                onLoaded: {
                                    item.objectName = chatBubbleID
                                }
                            }

                            Canvas {             
                                Layout.preferredWidth: isOpposite ? chatBubbleEdgeSize.width : 0
                                Layout.preferredHeight: isOpposite ? chatBubbleEdgeSize.height : 0
                                Layout.alignment: (isOpposite ? Qt.AlignRight : Qt.AlignLeft) | Qt.AlignTop

                                onPaint: {
                                    var context = getContext("2d")
                                    context.beginPath()
                                    context.moveTo(0,0)
                                    context.lineTo(0,height)
                                    context.lineTo(width,0)
                                    context.lineTo(0,0)
                                    context.closePath()
                                    context.fillStyle = "blue"
                                    context.fill()
                                }
                            }

                            Item {
                                Layout.fillWidth: !isOpposite
                            }
                        }
                    }

                    Component.onCompleted: {
                        chatListModel.append({  "isOpposite": false, 
                                                "chatBubbleID": "test", 
                                                "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml", 
                                                "contentData": String.raw`Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text` })

                        chatListModel.append({  "isOpposite": true, 
                                                "chatBubbleID": "test", 
                                                "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml", 
                                                "contentData": String.raw`Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text` })

                        chatListModel.append({  "isOpposite": false, 
                                                "chatBubbleID": "test", 
                                                "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml", 
                                                "contentData": String.raw`Text Test` })
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