import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: mainPage
    color: "#280a3d"
    objectName: "mainPage"

    property string currentRoomID

    var chatViewHash = {}

    function addChatRoom(chatRoomID, chatRoomName, chatRoomImage, recentUsedDate, recentChatContent)
    {
        chatRoomListModel.append({
            "chatRoomID": chatRoomID,
            "chatRoomName": chatRoomName,
            "chatRoomImage": "image://mybase64/data:image/png;base64," + chatRoomImage,
            "recentUsedDate": recentUsedDate,
            "recentChatContent": recentChatContent
        })


    }

    function addChatBubbleText(isRightAlign, userID, userName, userImage, chatData, chatTime)
    {
        chatListModel.append({
            "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml",
            "isRightAlign": isRightAlign,
            "userID": userID,
            "userName": userName,
            "userImage": userImage,
            "chatData": chatData,
            "chatTime": chatTime
        })
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
                    id: chatRoomID
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
                            color: "transparent"

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
                                color: "transparent"

                                Text {
                                    anchors.verticalCenter: parent.verticalCenter 
                                    anchors.left: parent.left
                                    anchors.right: recentUsedDateText.left
                                    anchors.rightMargin: 10
                                    clip: true
                                    text: chatRoomName
                                }

                                Text {
                                    id: recentUsedDateText
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    anchors.rightMargin: 5
                                    text: recentUsedDate
                                }                                
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "transparent"

                                Text {                               
                                    anchors.verticalCenter: parent.verticalCenter 
                                    anchors.left: parent.left                            
                                    text: recentChatContent
                                }                            
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            chatRoomID.color = "#BD5CF5"
                        }
                        onExited: {
                            chatRoomID.color = "#B240F5"
                        }
                        onClicked: {
                            
                        }
                    }
                }

                Component.onCompleted: {
                    for(var i=0;i < 15;i++) {
                        chatRoomListModel.append({
                            "chatRoomID": "rect" + i,
                            "chatRoomName": "Chat Room Num:" + i,
                            "recentChatContent": "Chat Room Preview",
                            "recentUsedDate": "0000-00-00"
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
                    // spacing: 6
                    boundsBehavior: Flickable.StopAtBounds

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    model: ListModel {
                        id: chatListModel
                    }

                    delegate: Item {
                        width: parent.width
                        height: chatBubbleLoader.height
                        objectName: userID // 어떤 사람이 연속으로 메시지를 보내고 있는지 알기 위함

                        // 말풍선 꼭다리 부분 크기
                        property var chatBubbleStemSize: Qt.size(11, 8)

                        // 말풍선 최대 너비
                        property var chatBubbleMaximumWidth: chatBubbleListView.width * 0.6

                        // 말풍선 최소 크기
                        property var chatBubbleMinimumSize: Qt.size(10, 10)

                        Loader {
                            id: chatBubbleLoader
                            width: parent.width
                            height: (item !== null && typeof(item) !== 'undefined') ? item.height : 0
                            source: chatBubbleSource

                            onLoaded: {
                                // item.objectName = chatBubbleID
                            }
                        }
                    }

                    onCountChanged: {
                    	Qt.callLater(positionViewAtEnd)
                    }

                    Component.onCompleted: {
                        addChatBubbleText(false, 
                                          "tongstar", 
                                          "이경준",
                                          "",
                                          "This is Test",
                                          "current time")

                        addChatBubbleText(false, 
                                          "tongstar", 
                                          "이경준",
                                          "",
                                          "And Test is keep goin",
                                          "current time")
                    }

                    // 밑 로직으로 날짜에 따라 채팅 읽어오기 가능
                    onContentYChanged: {
                        var currentIndexAtTop = indexAt(1, contentY)
                        console.log(currentIndexAtTop)
                        
                        var currentItem = itemAt(1, contentY)
                        console.log(currentItem.objectName)
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100

                    Flickable {
                        id: chatInputAreaFlickable
                        anchors.fill: parent
                        contentWidth: width
                        contentHeight: parent.height

                        TextArea.flickable: TextArea {
                            id: chatInputArea
                            width: chatInputAreaFlickable.width
                            height: chatInputAreaFlickable.height
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

                                // 서버로 채팅 내용 전송
                                mainPageContext.trySendTextChat(currentRoomID, text)

                                addChatBubbleText(true, 
                                                  "tongstar", 
                                                  "이경준",
                                                  "",
                                                  text,
                                                  "current time")
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