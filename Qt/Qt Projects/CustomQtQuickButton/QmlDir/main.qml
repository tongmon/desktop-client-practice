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
    title: "CustomQtQuickButton" // 윈도우 창 제목

    // 수평 레이아웃
    Row
    {
        // 해당 버튼 구조체의 이름은 무조건 qml 파일과 동일해야 함
        CustomButton
        {
            button_text: "my button 1"
            onButtonClicked: {          
                console.log("Clicked on Button 1")
            }
        }
        CustomButton
        {
            y: 50
            button_text: "my button 2"
        }
    }
}
