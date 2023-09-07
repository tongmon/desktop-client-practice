import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import ChatBubbleEnum 1.0

Rectangle {
    id: mainPage
    color: "#280a3d"
    objectName: "mainPage"

    function addChatBox(isRightAlign, contentType, content)
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
                    spacing: 6
                    // boundsBehavior: Flickable.StopAtBounds

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    model: ListModel {
                        id: chatListModel
                    }

                    delegate: Rectangle {
                        color: "transparent"
                        width: parent.width
                        height: chatBubbleLoader.item.height + chatBubbleNameText.paintedHeight + chatBubbleContentLayout.spacing

                        property var chatBubbleStemSize: Qt.size(11, 8)
                        property var chatBubbleMaximumWidth: chatBubbleListView.width * 0.6
                        property var chatBubbleMinimumSize: Qt.size(5, 5)

                        RowLayout {       
                            anchors.fill: parent
                            spacing: 0
                            
                            Item {
                                Layout.fillWidth: isRightAlign
                            }

                            Image {
                                source: "qrc:/icon/UserID.png"
                                visible: !isRightAlign
                                Layout.alignment: Qt.AlignTop
                                Layout.preferredWidth: 50
                                Layout.preferredHeight: 50
                            }
                            
                            ColumnLayout {
                                id: chatBubbleContentLayout
                                spacing: 3

                                Text {
                                    id: chatBubbleNameText
                                    text: "name"
                                    x: x + chatBubbleStemSize.width 
                                    visible: !isRightAlign
                                }

                                Loader {          
                                    id: chatBubbleLoader
                                    Layout.preferredWidth: (item !== null && typeof(item) !== 'undefined') ? item.width : 0
                                    Layout.preferredHeight: (item !== null && typeof(item) !== 'undefined') ? item.height : 0
                                    source: chatBubbleSource

                                    onLoaded: {
                                        item.objectName = chatBubbleID
                                    }
                                }
                            }
                            
                            Item {
                                Layout.fillWidth: !isRightAlign
                            }
                        }
                    }

                    onCountChanged: {
                    	Qt.callLater(positionViewAtEnd)
                    }

                    Component.onCompleted: {
                        chatListModel.append({  "isRightAlign": false, 
                                                "chatBubbleID": "test", 
                                                "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml", 
                                                "contentData": String.raw`Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test Text Test TextTest Text Test Text Test Text` })
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100

                    Flickable {
                        id: chatInputAreaFlickable
                        anchors.fill: parent
                        width: parent.width
                        height: parent.height
                        contentWidth: width
                        contentHeight: parent.height

                        TextArea.flickable: TextArea {
                            id: chatInputArea
                            anchors.fill: chatInputAreaFlickable
                            font.pointSize: 10
                            selectByMouse: true
                            wrapMode: TextEdit.Wrap

                            Keys.onReturnPressed: {
                                if(event.modifiers & Qt.ShiftModifier) {
                                    insert(cursorPosition, "\n")
                                    return
                                }

                                if(!text.length)
                                    return

                                chatListModel.append({  "isRightAlign": true, 
                                                        "chatBubbleID": "test", 
                                                        "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml", 
                                                        "contentData": text })
                                text = ""  
                            }                
                        }

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                        }
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