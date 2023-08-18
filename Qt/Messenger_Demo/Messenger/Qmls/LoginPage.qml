import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    color: "#280a3d"

    function checkLoginValidation()
    {
        if(4 < userIDTextField.text.length && 
           userIDTextField.text.length < 32 &&
           7 < passwordTextField.text.length && 
           passwordTextField.text.length < 64)
            return true
        return false
    }

    function tryLogin()
    {
        mainWindowLoader.source = "qrc:/qml/MainPage.qml"

        // 밑은 진짜 로그인 절차 로직, Boost Asio 사용하는 cpp 함수 호출 요망
        return loginPageContext.tryLogin(userIDTextField.text, passwordTextField.text)
    }

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