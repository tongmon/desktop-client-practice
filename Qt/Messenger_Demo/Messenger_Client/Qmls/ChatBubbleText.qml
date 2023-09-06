import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: chatBubbleTextRect
    color: "blue"

    Text {
        id: chatBubbleText
        anchors.verticalCenter: parent.verticalCenter 
        anchors.left: parent.left
        width: parent.width
        height: parent.height
        text: contentData
        wrapMode: Text.WordWrap
    }

    states: [
        State {
            name: "setMinimumWidth"
            when: chatBubbleText.paintedWidth < chatBubbleEdgeSize.width
            PropertyChanges {
                target: chatBubbleTextRect
                width: chatBubbleEdgeSize.width
                height: chatBubbleEdgeSize.height > chatBubbleText.paintedHeight ? chatBubbleEdgeSize.height : chatBubbleText.paintedHeight
            }
        },
        State {
            name: "setNormalWidth"
            when: chatBubbleEdgeSize.width <= chatBubbleText.paintedWidth && chatBubbleText.paintedWidth <= chatBubbleMaximumWidth
            PropertyChanges {
                target: chatBubbleTextRect
                width: chatBubbleText.paintedWidth
                height: chatBubbleEdgeSize.height > chatBubbleText.paintedHeight ? chatBubbleEdgeSize.height : chatBubbleText.paintedHeight
            }
        },
        State {
            name: "setMaximumWidth"
            when: chatBubbleText.paintedWidth > chatBubbleMaximumWidth
            PropertyChanges {
                target: chatBubbleTextRect
                width: chatBubbleMaximumWidth
                height: chatBubbleEdgeSize.height > chatBubbleText.paintedHeight ? chatBubbleEdgeSize.height : chatBubbleText.paintedHeight
            }
        }
    ]
}