import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: chatBubbleListViewRect
    anchors.fill: parent
    color: "transparent"

    // addChatBubbleText(isRightAlign, userID, userName, userImage, chatData, chatTime)
    // {
    //     chatListModel.append({
    //         "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml",
    //         "isRightAlign": isRightAlign,
    //         "userID": userID,
    //         "userName": userName,
    //         "userImage": userImage,
    //         "chatData": chatData,
    //         "chatTime": chatTime
    //     })
    // }

    ListView {
        id: chatBubbleListView
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
    
        model: ListModel {
            id: chatListModel
        }

        delegate: Item {
            width: chatBubbleListViewRect.width
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

            Component.onCompleted: {
                // console.log("chatbubble created! width: " + width + " height: " + height)
            }
        }

        onCountChanged: {
        	Qt.callLater(positionViewAtEnd)
        }

        Component.onCompleted: {
            console.log("ListView created! width: " + width + " height: " + height)

            // chatListModel.append({
            //     "chatBubbleSource": "qrc:/qml/ChatBubbleText.qml",
            //     "isRightAlign": true,
            //     "userID": "tongstar",
            //     "userName": "KyungJoonLee",
            //     "chatData": "test chat",
            //     "chatTime": "0000-00-00"
            // })

            // addChatBubbleText(true, objectName, "tongstar", "", "Hello " + objectName, "0000-00-00")
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