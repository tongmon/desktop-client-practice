import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Item {
    property int chatBubbleMargin: 3

    width: parent.width
    height: chatBubbleContentLayout.spacing + (isRightAlign ? 0 : chatBubbleNameText.paintedHeight) + chatBubbleBackground.height

    RowLayout {       
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.fillWidth: isRightAlign
        }

        Image {
            source: "qrc:/icon/UserID.png" // "data:image/png;base64," + userImage
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
                text: userName
                font.pixelSize: 15
                visible: !isRightAlign
                Layout.leftMargin: chatBubbleStemSize.width
            }

            Canvas {
                id: chatBubbleBackground
                Layout.preferredWidth: Math.max(chatBubbleTextItem.width, dateText.paintedWidth) + chatBubbleStemSize.width + chatBubbleMargin * 2
                Layout.preferredHeight: chatBubbleTextItem.height + dateText.paintedHeight + chatBubbleMargin * 2

                Item {
                    id: chatBubbleTextItem
                    x: isRightAlign ? chatBubbleMargin : (chatBubbleStemSize.width + chatBubbleMargin)
                    y: chatBubbleMargin
                    width: Math.min(chatBubbleMaximumWidth, Math.max(chatBubbleMinimumSize.width, dummyText.paintedWidth))
                    height: Math.max(chatBubbleText.paintedHeight, chatBubbleMinimumSize.height)

                    TextEdit {
                        id: chatBubbleText
                        anchors.verticalCenter: parent.verticalCenter 
                        anchors.left: parent.left
                        width: parent.width
                        height: parent.height
                        text: chatData
                        font.pixelSize: 15
                        selectByMouse: true
                        wrapMode: TextEdit.Wrap
                        readOnly: true
                    }

                    Text {
                        id: dummyText
                        text: chatBubbleText.text
                        wrapMode: chatBubbleText.wrapMode
                        font: chatBubbleText.font
                        visible: false
                    }
                }

                Text {
                    id: dateText
                    anchors.top: chatBubbleTextItem.bottom
                    anchors.left: isRightAlign ? chatBubbleTextItem.left : undefined
                    anchors.right: isRightAlign ? undefined : chatBubbleTextItem.right
                    text: chatTime
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

                    context.fillStyle = "white"
                    context.fill()
                }
            }
        }
                            
        Item {
            Layout.fillWidth: !isRightAlign
        }
    }
}