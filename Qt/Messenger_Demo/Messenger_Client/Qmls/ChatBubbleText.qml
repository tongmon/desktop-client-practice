import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    property string contentData
    property bool isOpposite

    color: "blue"

    Text {
        id: chatBubbleText
        anchors.verticalCenter: parent.verticalCenter 
        anchors.left: parent.left
        text: contentData
        wrapMode: Text.WordWrap

        // states: [
        //     State {
        //             name: "wide text"
        //             when: chatBubbleText.paintedWidth >= 100
        //             PropertyChanges {
        //                 target: chatBubbleText
        //                 width: 100
        //             }
        //     }
        // ]
    }
}