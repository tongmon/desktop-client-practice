import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item
{
    id: custom_button

    // width와 height을 적절히 지정해줘야 Colum, Row 같은 곳에 해당 버튼을 담을 때 의도한 위치에 있을 수 있음
    width: button_rect.width
    height: button_rect.height

    // 버튼 클래스의 멤버들 정보를 외부에서 사용하기 위해 alias를 지정함
    property alias button_text: button_title.text
    
    // 외부에서 직접 시그널 지정 가능
    signal buttonClicked
    
    Rectangle
    {
        id: button_rect
        width: button_title.implicitWidth + 20
        height: button_title.implicitHeight + 20
        color: "grey"
        border
        {
            color: "black"
            width: 3
        }
        Text 
        {
            id: button_title
            text: "Button"  
            anchors.centerIn: parent       
        }
        MouseArea
        {       
            anchors.fill: parent
            onClicked: {
                custom_button.buttonClicked()
            }
        }
    }
}
