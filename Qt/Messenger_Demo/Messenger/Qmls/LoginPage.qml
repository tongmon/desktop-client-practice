import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: applicationWindow
    visible: true
    x: 100
    y: 100
    width: 1024
    height: 768
    minimumWidth: 400
    minimumHeight: 300
    title: qsTr("Messenger")
    flags: Qt.Window
    
    // 창 크기 재조절 외각 부분, C++에서 받아서 처리
    property int resizeBorderWidth: 6

    Loader { 
        id: pageLoader 
    }

    function checkLoginValidation()
    {
        if(4 < userIDTextField.text.length && 
           userIDTextField.text.length < 32 &&
           12 < passwordTextField.text.length && 
           passwordTextField.text.length < 64)
            return true
        return false
    }

    function tryLogin()
    {
        pageLoader.source = "qrc:/qml/MainPage.qml"

        // 밑은 진짜 로그인 절차 로직, Boost Asio 사용하는 cpp 함수 호출 요망

    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: titleBar
            color: "#1f0830"

            Layout.fillWidth: true
            Layout.preferredHeight: 35
            Layout.alignment: Qt.AlignTop

            RowLayout {
                anchors.fill: parent
                spacing: 0

                component TitleBarMenuItem : MenuItem {
                    id: menuItem
                    implicitHeight: titleBar.height
                    contentItem: Text {
                        text: menuItem.text
                        font: menuItem.font
                        color: active ? (menuItem.highlighted ? "#ffffff" : Qt.rgba(0.8, 0.8, 0.8, 1.0)) : Qt.rgba(0.5, 0.5, 0.5, 1.0)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    background: Rectangle {
                        anchors.fill: parent
                        opacity: menuItem.highlighted ? 0.7 : 1.0
                        color: "#170624"
                    }
                }

                // 윈도우 드래그 가능 상태로 만들어주는 부분
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    MouseArea {
                        anchors.fill: parent

                        // 누르고 있는 경우 startSystemMove() 함수 수행
                        // startSystemMove()는 윈도우를 드래그 가능 상태로 만들고 aero snap, aero shake와 같은 윈도우 기본 기능도 부여함
                        onPressed: {
                            applicationWindow.startSystemMove()
                        }

                        // startSystemMove()를 하더라도 더블 클릭시 maximize / restore 기능은 부여하지 않기에 직접 구현
                        onDoubleClicked: {
                            applicationWindow.visibility = maximumButton.checked ? Window.Windowed : Window.Maximized
                        }
                    }
                }

                Button {
                    id: minimumButton
                    Layout.preferredWidth: 46
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(1.0, 1.0, 1.0, 0.4) : (parent.hovered ? Qt.rgba(1.0, 1.0, 1.0, 0.2) :
                                                                            Qt.rgba(1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: active ? "qrc:/icon/Minimize.png" : "qrc:/icon/MinimizeDeactivated.png"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }

                    // C++에 구현되어 있는 최소화 버튼에 대한 Invoke 함수 onMinimizeButtonClicked()를 수행
                    onClicked: {
                        cppConnector.onMinimizeButtonClicked()
                    }
                }

                Button {
                    id: maximumButton
                    objectName: "maximumButton"
                    Layout.preferredWidth: 46
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    checkable: true
                    checked: false
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(1.0, 1.0, 1.0, 0.4) : (parent.hovered ? Qt.rgba(1.0, 1.0, 1.0, 0.2) :
                                                                                             Qt.rgba(1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: maximumButton.checked ? (active ? "qrc:/icon/Restore.png" : "qrc:/icon/RestoreDeactivated.png") :
                                                        (active ? "qrc:/icon/Maximize.png" : "qrc:/icon/MaximizeDeactivated.png")
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }

                    // C++에 구현되어 있는 최대/복구 버튼에 대한 Invoke 함수 onMaximizeButtonClicked()를 수행
                    onClicked: {
                        cppConnector.onMaximizeButtonClicked()
                    }
                }

                Button {
                    id: closeButton
                    Layout.preferredWidth: 46
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(0.78, 0.16, 0.184, 0.6) : (parent.hovered ? Qt.rgba(0.86, 0.16, 0.184, 0.9) :
                                                                                                 Qt.rgba(1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: closeButton.hovered ? "qrc:/icon/CloseHoverOrPressed.png" :
                                                      (active ? "qrc:/icon/Close.png" : "qrc:/icon/CloseDeactivated.png")
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }

                    // C++에 구현되어 있는 닫기 버튼에 대한 Invoke 함수 onMaximizeButtonClicked()를 수행
                    onClicked: {
                        cppConnector.onCloseButtonClicked()
                    }
                }
            }
        }

        Rectangle {
            id: mainContent
            color: "#280a3d"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredHeight: 35
                    text: qsTr("User Login")
                }

                Item {
                    Layout.preferredHeight: 20
                    Layout.fillWidth: true
                }

                Rectangle {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 275
                    radius: 10

                    Image {
                        id: userIDImage
                        anchors {
                            left: parent.left              
                            top: parent.top
                            bottom: parent.bottom
                        }
                        width: parent.height
                        source: "qrc:/icon/UserID.png" // from https://www.pngwing.com/
                    }

                    TextField {
                        id: userIDTextField
                        placeholderText: "User ID"
                        selectByMouse: true
                        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                        // validator: IntValidator { 
                        //     bottom: 0 
                        //     top: 31
                        // }

                        anchors {
                            left: userIDImage.right                
                            top: parent.top
                            right: parent.right
                            bottom: parent.bottom
                        }

                        Keys.onReturnPressed: {
                            if (checkLoginValidation())
                                tryLogin()
                        }

                        background: Rectangle {
                            color: "transparent"
                        }
                    }
                }

                Item {
                    Layout.preferredHeight: 10
                    Layout.fillWidth: true
                }

                Rectangle {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 275
                    radius: 10

                    Image {
                        id: passwordImage
                        anchors {
                            left: parent.left              
                            top: parent.top
                            bottom: parent.bottom
                        }
                        width: parent.height
                        source: "qrc:/icon/Password.png"
                    }

                    TextField {
                        id: passwordTextField
                        placeholderText: "Password"
                        selectByMouse: true
                        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText

                        anchors {
                            left: passwordImage.right                
                            top: parent.top
                            right: parent.right
                            bottom: parent.bottom
                        }

                        Keys.onReturnPressed: {
                            if (checkLoginValidation())
                                tryLogin()
                        }

                        background: Rectangle {
                            color: "transparent"
                        }
                    }
                }

                Item {
                    Layout.preferredHeight: 10
                    Layout.fillWidth: true
                }

                Row {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 275

                    CheckBox {
                        text: qsTr("Remember")
                    }
                }

                Item {
                    Layout.preferredHeight: 20
                    Layout.fillWidth: true
                }

                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: qsTr("Login")

                    onClicked: {
                        if (checkLoginValidation())
                            tryLogin()
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }
    }
}
