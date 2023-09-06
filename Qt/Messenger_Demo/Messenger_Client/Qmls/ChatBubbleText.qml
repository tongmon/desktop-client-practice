import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: chatBubbleTextRect
    color: "blue"
    width: {
        if(chatBubbleText.paintedWidth < chatBubbleEdgeSize.width) 
            return chatBubbleEdgeSize.width
        else if(chatBubbleText.paintedWidth >= chatBubbleEdgeSize.width)
            return chatBubbleText.paintedWidth
        else
            return 300
    }
    height: chatBubbleText.paintedHeight < chatBubbleEdgeSize.height ? chatBubbleEdgeSize.height : chatBubbleText.paintedHeight

    Text {
        id: chatBubbleText
        anchors.verticalCenter: parent.verticalCenter 
        anchors.left: parent.left
        text: contentData
        wrapMode: Text.WordWrap
    }

    // states: [
    //     State {
    //         name: "setMinimumWidth"
    //         when: chatBubbleTextRect.width < chatBubbleEdgeSize.width
    //         PropertyChanges {
    //             target: chatBubbleTextRect
    //             width: chatBubbleEdgeSize.width
    //         }
    //     },
    //     State {
    //         name: "setNormalWidth"
    //         when: chatBubbleEdgeSize.width <= chatBubbleTextRect.width && chatBubbleTextRect.width <= 300
    //         PropertyChanges {
    //             target: chatBubbleTextRect
    //             width: chatBubbleTextRect.width
    //         }
    //     },
    //     State {
    //         name: "setMaxmimumWidth"
    //         when: chatBubbleTextRect.width > 300
    //         PropertyChanges {
    //             target: chatBubbleTextRect
    //             width: 300
    //         }
    //     },
    //     State {
    //         name: "setMinimumHeight"
    //         when: chatBubbleTextRect.height < chatBubbleEdgeSize.height
    //         PropertyChanges {
    //             target: chatBubbleTextRect
    //             height: chatBubbleEdgeSize.height
    //         }
    //     },
    //     State {
    //         name: "setNormalHeight"
    //         when: chatBubbleTextRect.height >= chatBubbleEdgeSize.height
    //         PropertyChanges {
    //             target: chatBubbleTextRect
    //             height: chatBubbleTextRect.height
    //         }
    //     }
    // ]
}