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

