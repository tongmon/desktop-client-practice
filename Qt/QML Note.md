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
&nbsp;  

QML에서 가장 중요한 Property Binding에 대해 다뤄보자.  
Property Binding은 거창한 것은 아니고 ui가 서로 맞물려 돌아갈 수 있도록 해주는 장치라고 볼 수 있다.  
코드를 보면 이해가 쉽다.  
```qml
Window {
    Rectangle {
        id: red_rect
        width: 50
        height: width * 1.5
        color: "red"
    }
}
```
위에서 ```height: width * 1.5``` 부분을 잘 보자.  
가로가 변하면 세로가 그에 맞춰져 변한다.  
&nbsp;  

아직 감이 안왔을 수 있으니 좀 더 보자.  
```qml
Window {
    Rectangle {
        id: red_rect
        width: 50
        height: width * 1.5
        color: "red"
    }

    Rectangle {
        id: blue_rect
        width: 50
        height: 50
        color: "blue"
        anchors.bottom: parent.bottom
        
        MouseArea{           
            anchors.fill: parent
            onClicked: {
                red_rect.width = red_rect.width + 10
            }
        }
    }
}
```
파란색 사각형을 추가하고 해당 사각형을 누르면 빨간 사각형의 너비가 바뀌게 된다.  
너비가 바뀌면 알아서 높이도 맞춰 변하게 된다.  
```red_rect.width = red_rect.width + 10```처럼 변수를 바꾸기만 해도 이벤트가 알아서 발생하여 해당 객체에 영향을 주는 것이 Property Binding이다.  
&nbsp;  

Property Binding에서 주의할 점은 코드를 짜다가 바인딩을 박살낼 수가 있다는 점이다.  
밑의 코드를 보자.  
```qml
Window {
    // 전과 동일

    Rectangle {
        id: green_rect
        color: "green"
        width: 50
        height: 50
        anchors.bottom: parent.bottom        
        anchors.left: blue_rect.right
        
        MouseArea{           
            anchors.fill: parent
            onClicked: {
                red_rect.height = 100
            }
        }
    }
}
```
파란색 사각형 옆에 녹색 사각형을 추가했다.  
녹색 사각형을 누르면 red_rect의 ```height: width * 1.5``` 규칙이 망가지고 red_rect의 높이는 100으로 고정된다.  
즉 파란색 사각형을 눌러도 가로만 변하고 세로는 고정이다.  
&nbsp;  

그렇다면 ```red_rect.height = 100``` 부분을 ```red_rect.height = red_rect.width * 1.5```로 바꾸면 해결이 될까?  
해결이 안된다.  
그저 녹색 버튼이 눌린 시점의 ```red_rect.width * 1.5``` 값으로 red_rect의 높이가 고정이 될 뿐이다.  
&nbsp;  

이를 해결하기 위해선 바인딩을 다시 짜줘야 한다.  
```qml
Window {
    // 전과 동일

    Rectangle {
        id: green_rect
        color: "green"
        width: 50
        height: 50
        anchors.bottom: parent.bottom        
        anchors.left: blue_rect.right
        
        MouseArea{           
            anchors.fill: parent
            onClicked: {
                red_rect.height = Qt.binding(
                    function() {
                        return red_rect.width * 2
                    }
                )
            }
        }
    }
}
```
코드를 보면 Qt.binding에 특정 함수를 구현해 바인딩하는 것을 볼 수 있다.
이제 녹색 사각형을 누르면 ```red_rect.width * 1.5``` 규칙은 ```red_rect.width * 2```로 바뀐다.  
바인딩이 변경된 것이기에 red_rect의 가로 길이에 따라 세로 길이가 변한다.  
&nbsp;  

프로퍼티 값이 변경될 때마다 작동하는 함수를 만드려면 어떻게 해야할까?  
```qml
Window {
    property string mMyName: "tongstar"
    onMMyNameChanged: {
        console.log("My Name is changed to :" + mMyName)
    }
    
    Rectangle{
        width: 300
        height: 100
        color: "green"
        anchors.centerIn: parent

        MouseArea{          
            anchors.fill: parent
            onClicked: {
                mMyName = "kyungjoonlee"
            }
        }
    }
}
```
여기서 사각형을 누르면 mMyName이 바뀌게 되고 그때 onMMyNameChanged 함수가 수행이 된다.  
해당 함수의 이름 규칙은 변수명을 따라가는데 일단 변수명이 mMyName이기에 mMyName를 사용은 하되 맨 앞글자는 대문자다.  
그리고 앞에는 on이, 뒤에는 Changed가 붙는다.  
최종적으로 onMMyNameChanged 라는 이름의 함수를 만들면 해당 함수는 mMyName이 변경될 때마다 작동하게 된다.  
항상 프로퍼티 변수명의 첫 글자가 소문자인 경우 작동하는 기믹이기에 이를 유의하자.  
&nbsp;  

Item이라는 녀석도 존재한다.  
특정 UI 객체들을 모아두는 녀석이라고 보면 된다.  
```qml
Window {
    // 생략 

    Item{
        x: 10
        y: 10
        width: 300
        height: 200

        // Item 배경색 지정을 위한 사각형
        Rectangle {
            id: item_bckground
            anchors.fill: parent
            color: "beige"
        }

        // Item 내부에 위치하기에 실제 절대 위치는 x: 20, y: 20이다.
        Rectangle {
            id: green_rect
            x: 10
            y: 10
            width: 50
            height: 50
            color: "green"
        }

        Rectangle {
            id: red_rect
            width: 50
            height: 50
            color: "red"
            anchors {
                left: green_rect.right
                leftMargin: 10
                top: green_rect.top
            }
        }
    }
}
```
Item에 위와 같이 UI 객체들을 손쉽게 묶어 관리할 수 있다.  
&nbsp;  

위에서 본 예시의 계층을 살펴보면 Item이 부모고 자식 UI는 Rectangle이 된다.  
만약 자식 UI의 크기를 부모 UI에서 알고 싶다면 어떻게 해야 할까?  
implicitWidth, implicitHeight를 이용하면 된다.  
```qml
Window {
    // 생략

    Rectangle{
        width: my_text.implicitWidth + 20;
        height: my_text.implicitHeight + 20;
        color: "beige"
        border {
            color: "grey"
            width: 3
        }
        Text {
            id: my_text
            text: "My Text"   
            anchors.centerIn: parent
            font.pointSize: 30
        }
    }
}
```
위 예시에서는 Text의 UI 크기의 가로, 세로에 20이 더해진 크기로 Rectangle이 생성된다.  
즉 자식 UI의 크기를 이용하고 싶을 때 implicitWidth, implicitHeight를 적절히 사용하면 된다.  
&nbsp;  

Qml에서 signal와 slot을 어떻게 다루는지 알아보자.  
```qml
Window {
    Rectangle{
        id: my_rect
        width : 100
        height: 100
        color: "beige"

        // 사용자 정의 시그널을 만든다.
        signal my_custom_signal(string signal_msg)

        // 함수명 첫글자는 무조건 소문자여야 한다.
        // 모든 자바스크립트 함수는 다 가능
        function myCustomFunction(signal_msg) {
            console.log("Signal Message: " + signal_msg)
        }

        MouseArea
        {           
            anchors.fill: parent
            onClicked: {
                // 사용자 정의 시그널을 발동한다.  
                my_rect.my_custom_signal("Message From Beige Rectangle!")
            }
        }

        // Rectangle 생성될 때 수행되는 생성자와 비슷한 함수
        Component.onCompleted: {
            // 커스텀 시그널과 특정 JS 함수와 연결한다. 
            // my_custom_signal 시그널이 발생하면 myCustomFunction 함수가 수행된다.
            my_custom_signal.connect(myCustomFunction)
        }
    }
}
```
위와 같이 따로 JS 함수를 생성하고 connect 시키는 방식이 있다.  
id만 알면 외부 객체 각자의 멤버 함수들을 connect 할 수 있기에 유연하다.  
&nbsp;  

밑과 같이 자동으로 생성된 slot을 이용하는 방법도 있다.  
```qml
Window {
    Rectangle{
        id: my_rect
        width : 100
        height: 100
        color: "beige"

        signal my_custom_signal(string signal_msg)

        // on이 붙고 시그널의 첫문자가 대문자로 변한다.
        // 시그널에서 전달한 모든 인자를 사용할 수 있다.  
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
    }
}
```
위 처럼 따로 JS 함수를 만들지 않고 시그널을 이용할 수도 있다.  
&nbsp;  

## 위치 잡기    

### Anchors  

```anchors```는 특정 객체의 상대적 위치를 정하는 데 필요하다.  
일일이 x, y를 지정해주는 것보다 기준 객체를 정하고 해당 기준 객체에서 얼마만큼 떨어져 있는지를 표현하는 것이 용이하기 때문에 존재한다.  
해당 특성은 ```anchors.left```, ```anchors.top```, ```anchors.right```, ```anchors.bottom``` 등으로 지정할 수 있다.  

```anchors.verticalCenter```, ```anchors.horizontalCenter```, ```anchors.centerIn``` 등의 특성도 존재한다.  
특성 이름에 맞게 수직 정렬, 수평 정렬, 중앙 정렬을 나타낸다.  

```anchors``` 내부 특성인 ```margin```은 ```anchors```로 정해진 기준점으로 부터 정확히 얼마나 떨어져 있는지 판단하기 위해 사용된다.   
예를 들어 item2 객체 내부에 ```anchors.left: item1.right```이고 ```anchors.leftMargin: 5```라고 정의되어 있다면 item2는 item1 오른쪽 옆에 붙은 상태에서 5만큼 떨어져있게 된다.  

주의할 점은 anchors는 상대적인 위치를 이용하긴 하지만 상대적인 객체에 대한 제약이 있다.  
parent, child, sibling에 한하여 상대적 위치 설정이 가능하다.  
쌩뚱맞게 parent의 sibling을 기준으로 ```anchors```를 설정해봤자 적용이 안된다.  
&nbsp;  

### Positioners  

Grid, Row, Column 등의 객체는 anchors 특성을 지정할 필요없이 자동적으로 anchors 특성이 적용된다.  
오히려 anchors를 적용하면 위치 정렬이 망가진다.  
margin은 위 객체들에서 spacing으로 처리할 수 있다.  

내부 아이템 정렬 설정은 Grid에서 예를 들어보면 ```horizontalItemAlignment: Grid.AlignRight```, ```verticalItemAlignment: Grid.AlignVCenter``` 등으로 설정할 수 있다.  
