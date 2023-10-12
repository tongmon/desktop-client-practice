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

    property var chatViewObjects: ({})

    property var chatListViewComponent: Qt.createComponent("qrc:/qml/ChatListView.qml")

    function addChatRoom(chatRoomID, chatRoomName, chatRoomImage, recentUsedDate, recentChatContent)
    {
        chatRoomListModel.append({
            "chatRoomObjectName": chatRoomID,
            "chatRoomName": chatRoomName,
            "chatRoomImage": "image://mybase64/data:image/png;base64," + chatRoomImage,
            "recentUsedDate": recentUsedDate,
            "recentChatContent": recentChatContent
        })

        chatViewObjects[chatRoomID] = chatListViewComponent.createObject(null)
        // chatViewObjects[chatRoomID].width = Qt.binding(function() { return chatView.width })
        // chatViewObjects[chatRoomID].height = Qt.binding(function() { return chatView.height })

        // if(chatRoomID === "test_01")
        //     chatViewObjects[chatRoomID].color = "blue"
    }

    function addChatBubbleText(chatRoomID, isRightAlign, userID, userName, userImage, chatData, chatTime)
    {
        // chatViewObjects[chatRoomID]
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
                            // chatViewHash[chatRoomObjectName] = index
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
                            currentRoomID = chatRoomID.objectName

                            if(chatView.empty)
                                chatView.push(chatViewObjects[currentRoomID], StackView.Immediate)
                            else
                                chatView.replace(null, chatViewObjects[currentRoomID], StackView.Immediate)

                            console.log("chat view width: " + chatViewObjects[currentRoomID].width + " height: " + chatViewObjects[currentRoomID].height)

                            // chatListViewLoader.sourceComponent = chatViewObjects[currentRoomID]

                            // chatListView.itemAtIndex(chatViewHash[currentRoomID]).visible = true
                            // chatListView.positionViewAtIndex(chatViewHash[currentRoomID], ListView.Beginning)
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

                StackView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: chatView
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

                                chatViewObjects[currentRoomID].children[0].model.append({
                                    "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml",
                                    "isRightAlign": true,
                                    "userID": "tongstar",
                                    "userName": "KyungJoonLee",
                                    "chatData": text,
                                    "chatTime": "0000-00-00"
                                })

                                //addChatBubbleText(currentRoomID,
                                //                  true,
                                //                  "tongstar",
                                //                  "KyungJoonLee",
                                //                  "",
                                //                  text,
                                //                  "current time")       

                                // console.log(chatListViewLoader.sourceComponent.objectName)

                                // console.log(chatViewObjects[currentRoomID].children[0].objectName)

                                // chatViewObjects[currentRoomID].children[0].model.append({
                                //     "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml",
                                //     "isRightAlign": isRightAlign,
                                //     "userID": userID,
                                //     "userName": userName,
                                //     "userImage": userImage,
                                //     "chatData": chatData,
                                //     "chatTime": chatTime
                                // })

                                // chatViewObjects[chatRoomID].addChatBubbleText(true, "tongstar", "KyungJoonLee", "", text, "current time")       

                                // 컴포넌트 내부에서 함수 적용되게 해야됨...

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
