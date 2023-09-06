import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: chatBubbleTextRect
    color: "blue"
    width: {
        if(dummyText.paintedWidth < chatBubbleEdgeSize.width)
            return chatBubbleEdgeSize.width
        else if(dummyText.paintedWidth > chatBubbleMaximumWidth)
            return chatBubbleMaximumWidth
        else
            return dummyText.paintedWidth
    }
    height: Math.max(chatBubbleText.paintedHeight, chatBubbleEdgeSize.height)

    Text {
        id: chatBubbleText
        anchors.verticalCenter: parent.verticalCenter 
        anchors.left: parent.left
        width: parent.width
        height: parent.height
        text: contentData
        wrapMode: Text.WordWrap
    }

    Text {
        id: dummyText
        text: chatBubbleText.text
        wrapMode: chatBubbleText.wrapMode
        visible: false
    }
}