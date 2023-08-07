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
    title: qsTr("FrameLess Window")
    flags: Qt.Window
    
    // 창 크기 재조절 외각 부분, C++에서 받아서 처리
    property int resizeBorderWidth: 6

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: titleBar
            color: Qt.rgba(0.117, 0.133, 0.152, 1.0)

            Layout.fillWidth: true
            Layout.preferredHeight: 35
            Layout.alignment: Qt.AlignTop

            RowLayout {
                anchors.fill: parent
                spacing: 0

                MenuBar {
                    id: titleMenuBar
                    Layout.alignment: Qt.AlignLeft
                    Menu {
                        title: "File"
                        Action {
                            text: "New..."
                        }
                        Action {
                            text: "Open..."
                        }
                        Action {
                            text: "Save"
                        }
                        Action {
                            text: "Save As..."
                        }
                        MenuSeparator {}
                        Action {
                            text: "Quit"
                        }
                    }
                    Menu {
                        title: "Edit"
                        Action {
                            text: "Cut"
                        }
                        Action {
                            text: "Copy"
                        }
                        Action {
                            text: "Paste"
                        }
                    }
                    Menu {
                        title: "Help"
                        Action {
                            text: "About"
                        }
                    }
                    delegate: MenuBarItem {
                        id: menuBarItem
                        contentItem: Text {
                            text: menuBarItem.text
                            font: menuBarItem.font
                            color: active ? Qt.rgba(0.8, 0.8, 0.8, 1.0) : Qt.rgba(0.5, 0.5, 0.5, 1.0)
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        background: Rectangle {
                            implicitHeight: titleBar.height
                            color: menuBarItem.highlighted ? Qt.rgba(1.0, 1.0, 1.0, 0.2) : "transparent"
                        }
                    }
                    background: Rectangle {
                        implicitHeight: titleBar.height
                        color: "transparent"
                        Rectangle {
                            color: "#21be2b"
                            width: parent.width
                            height: 1
                            anchors.bottom: parent.bottom
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            applicationWindow.startSystemMove()
                        }
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
                    onClicked: {
                        cppConnector.onCloseButtonClicked()
                    }
                }
            }
        }

        Rectangle {
            id: mainContent
            color: Qt.rgba(0.137, 0.152, 0.18, 1.0)
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom
            Button {
                text: "hi"
                anchors.centerIn: parent
            }
        }
    }
}
