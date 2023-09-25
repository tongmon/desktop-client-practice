import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: mainPage
    color: "#280a3d"
    objectName: "mainPage"

    property string currentRoomID: ""

    // chatRoomID와 ListView 인덱스 연관 관계 정의
    // chat room 삭제 시에 일괄 for문 돌면서 지우는 로직 추가해야 됨
    property var chatViewHash: ({})

    function addChatRoom(chatRoomID, chatRoomName, chatRoomImage, recentUsedDate, recentChatContent)
    {
        chatRoomListModel.append({
            "chatRoomObjectName": chatRoomID,
            "chatRoomName": chatRoomName,
            "chatRoomImage": "image://mybase64/data:image/png;base64," + chatRoomImage,
            "recentUsedDate": recentUsedDate,
            "recentChatContent": recentChatContent
        })

        chatListModel.append({"chatBubbleListObjectName": chatRoomID})
    }

    function addChatBubbleText(chatRoomID, isRightAlign, userID, userName, userImage, chatData, chatTime)
    {
        chatListView.itemAtIndex(chatViewHash[chatRoomID]).children[0].model.append({
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
                    property int chatRoomIndex: index

                    id: chatRoomID
                    objectName: chatRoomObjectName
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
                                    source: chatRoomImage // "qrc:/icon/UserID.png" 
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

                        Component.onCompleted: { 
                            chatViewHash[chatRoomObjectName] = index
                        }

                        Component.onDestruction: {

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
                            if(currentRoomID.length > 0)
                                chatListView.itemAtIndex(chatViewHash[currentRoomID]).visible = false

                            currentRoomID = chatRoomID.objectName

                            chatListView.itemAtIndex(chatViewHash[currentRoomID]).visible = true
                            chatListView.positionViewAtIndex(chatViewHash[currentRoomID], ListView.Beginning)
                        }
                    }
                }

                Component.onCompleted: {           
                    addChatRoom("test_01", "chat room 1", "", "1997-03-09", "chat preview in chat room 1")

                    addChatRoom("test_02", "chat room 2", "", "2023-09-21", "chat preview in chat room 2")

                    // for(var i=0;i < 15;i++) {
                    //     chatRoomListModel.append({
                    //         "chatRoomID": "rect" + i,
                    //         "chatRoomName": "Chat Room Num:" + i,
                    //         "recentChatContent": "Chat Room Preview",
                    //         "recentUsedDate": "0000-00-00"
                    //         })
                    // }
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
                    id: chatListView
                    clip: true
                    interactive: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    model: ListModel {
                        id: chatListModel
                    }

                    delegate: Rectangle {
                        width: parent.width
                        height: chatListView.height
                        color: "transparent"

                        ListView {
                            id: chatBubbleListView
                            objectName: chatBubbleListObjectName
                            anchors.fill: parent
                            clip: true
                            boundsBehavior: Flickable.StopAtBounds                    
                            // spacing: 6

                            ScrollBar.vertical: ScrollBar {
                                policy: ScrollBar.AsNeeded
                            }

                            model: ListModel {
                                id: chatBubbleListModel
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

                            }

                            // 밑 로직으로 날짜에 따라 채팅 읽어오기 가능
                            // onContentYChanged: {
                            //     var currentIndexAtTop = indexAt(1, contentY)
                            //     console.log(currentIndexAtTop)
                            //
                            //     var currentItem = itemAt(1, contentY)
                            //     console.log(currentItem.objectName)
                            // }                        
                        }
                    }

                    // 화면 크기 조정시 chat bubble 위치 바로 잡기
                    // 아직 밑과 같은 문제 남아있음
                    // 1. 창 크기 확대시 chat bubble 이상함
                    // 2. 가로로 좀 많이 축소시 chat bubble 이상함
                    onWidthChanged: {
                        chatListView.positionViewAtIndex(chatViewHash[currentRoomID], ListView.Beginning)
                    }

                    onHeightChanged: {
                        chatListView.positionViewAtIndex(chatViewHash[currentRoomID], ListView.Beginning)
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

                                addChatBubbleText(currentRoomID,
                                                  true,
                                                  "tongstar",
                                                  "KyungJoonLee",
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
