# QML 메모  

QML에 대해 공부하면서 대략적인 사용법을 정리하였다.  
예제를 중심으로 코드를 보면서 설명하겠다.  

QML로 윈도우를 생성할 때 대략적인 모습은 밑과 같다.  
```qml
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
}
```
JS 엔진을 기반으로 하기에 헤더 포함은 import로 수행한다.  
위 말고도 더 많은 프로퍼티가 존재한다.  
[이곳](https://doc.qt.io/qt-5/qml-qtquick-window-window.html)에서 볼 수 있다.  
&nbsp;  

위 상태에서 내부에 문자열이 담긴 텍스트를 만들어보자.  
```qml
Window {
    // 전과 동일

    Rectangle {
        width: 200
        height: 100
        anchors.centerIn: parent
        color: "yellow"
        
        Text {
            id: mTextId // 고유 id를 지어줌, 변수 이름이라고 보면 된다.
            anchors.centerIn: parent
            text: qsTr("Hello World")
        }
    }
}
```
위에서 생소한 건 anchors 그리고 qsTr 정도가 되겠다.  
anchors는 아이템의 상대적인 위치가 어느 곳인지 알려주기 위한 것이다.  
위에서 ```anchors.centerIn: parent```를 해줬으므로 Rectangle의 부모인 Window의 중앙에 Rectangle이 위치하게 된다.  
qsTr은 문자열을 현지화 하기위해 존재한다.  
QML 현지화에 대한 자세한 정보는 [이곳](https://doc.qt.io/qt-5/qtquick-internationalization.html)에서 볼 수 있다.  
&nbsp;  

QML에서 변수를 선언해보자.  
```qml
Window {
    // 전과 동일

    property string myString: "My String"
    property int myInt: 1
    property bool myBool: true
    property var myPoint: Qt.point(10,10)
    property var myColor: Qt.rgba(1,1,1,0)
}
```
이렇게 선언할 수 있다.  
더 많은 자료형은 [이곳](https://doc.qt.io/qt-5/qtqml-typesystem-basictypes.html)에서 볼 수 있다.  