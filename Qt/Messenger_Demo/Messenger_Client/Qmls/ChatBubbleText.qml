import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Canvas {
    width: chatBubbleTextRect.width + chatBubbleStemSize.width + 6
    height: chatBubbleTextRect.height + 6

    Rectangle {
        id: chatBubbleTextRect
        x: isRightAlign ? 3 : (chatBubbleStemSize.width + 3)
        y: 3
        color: "transparent"
        width: Math.min(chatBubbleMaximumWidth, Math.max(chatBubbleMinimumSize.width, dummyText.paintedWidth))
        height: Math.max(chatBubbleText.paintedHeight, chatBubbleMinimumSize.height)

        Text {
            id: chatBubbleText
            anchors.verticalCenter: parent.verticalCenter 
            anchors.left: parent.left
            width: parent.width
            height: parent.height
            text: contentData
            wrapMode: Text.WordWrap
            font.pixelSize: 15
        }

        Text {
            id: dummyText
            text: chatBubbleText.text
            wrapMode: chatBubbleText.wrapMode
            font: chatBubbleText.font
            visible: false
        }
    }

    onPaint: {
        var context = getContext("2d")
        context.beginPath()

        if(isRightAlign) {
            context.moveTo(width - chatBubbleStemSize.width, 0)
            context.lineTo(width - chatBubbleStemSize.width, 5)
            context.lineTo(width, 5)
            context.lineTo(width - chatBubbleStemSize.width, 5 + chatBubbleStemSize.height)
            context.lineTo(width - chatBubbleStemSize.width, height)
            context.lineTo(0, height)
            context.lineTo(0, 0)
            context.lineTo(width - chatBubbleStemSize.width, 0)
        }
        else {
            context.moveTo(chatBubbleStemSize.width, 0)
            context.lineTo(chatBubbleStemSize.width, 5)
            context.lineTo(0, 5)
            context.lineTo(chatBubbleStemSize.width, 5 + chatBubbleStemSize.height)
            context.lineTo(chatBubbleStemSize.width, height)
            context.lineTo(width, height)
            context.lineTo(width, 0)
            context.lineTo(chatBubbleStemSize.width, 0)
        }

        context.fillStyle = "blue"
        context.fill()
    }
}