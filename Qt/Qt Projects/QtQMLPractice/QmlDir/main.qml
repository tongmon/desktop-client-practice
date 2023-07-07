import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    visible: true // 윈도우 보임 여부
    width: 500 // 윈도우 초기 너비   
    height: 300 // 윈도우 초기 높이
    minimumWidth: 200 // 윈도우 최소 너비
    minimumHeight: 100 // 윈도우 최대 너비
    title: "CMake and Qt Quick" // 윈도우 창 제목

    Rectangle{
        id: my_rect
        width : 100
        height: 100
        color: "beige"

        signal my_custom_signal(string signal_msg)

        // 함수명 첫글자는 무조건 소문자여야 한다.
        // function myCustomFunction(signal_msg) {
        //     console.log("Signal Message: " + signal_msg)
        // }

        onMy_custom_signal: {
            console.log("Signal Message: " + signal_msg)
        }

        MouseArea
        {           
            anchors.fill: parent
            onClicked: {
                my_rect.my_custom_signal("Message From Beige Rectangle!")
            }
        }

        Component.onCompleted: {
            // my_custom_signal.connect(myCustomFunction)
        }
    }
}
