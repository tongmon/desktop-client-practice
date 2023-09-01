import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    color: "#280a3d"
    objectName: "loginPage"

    // 비밀번호, 아이디 유효성 검사
    function checkLoginValidation()
    {
        if(4 < userIDTextField.text.length && userIDTextField.text.length < 32 &&
           7 < passwordTextField.text.length && passwordTextField.text.length < 64)
            return true
        return false
    }

    // 로그인 성공시 화면 전환
    function successLogin()
    {
        mainWindowLoader.source = "qrc:/qml/MainPage.qml"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TitleBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 35
            Layout.alignment: Qt.AlignTop
        }

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
                
                validator: RegularExpressionValidator { 
                    regularExpression: /[0-9a-zA-Z]+/
                }

                anchors {
                    left: userIDImage.right                
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                }

                Keys.onReturnPressed: {
                    if (checkLoginValidation())
                        loginPageContext.tryLogin(userIDTextField.text, passwordTextField.text)
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
                echoMode: TextField.Password // echoMode: showText ? TextField.Normal : TextField.Password
                passwordCharacter: "●"
                
                anchors {
                    left: passwordImage.right                
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                }

                Keys.onReturnPressed: {
                    if (checkLoginValidation())
                        loginPageContext.tryLogin(userIDTextField.text, passwordTextField.text)
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
                    loginPageContext.tryLogin(userIDTextField.text, passwordTextField.text)
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    Component.onCompleted: {
        console.log("Login Page Start!")
    }

    Component.onDestruction: {
        console.log("Login Page End!")
    }
}