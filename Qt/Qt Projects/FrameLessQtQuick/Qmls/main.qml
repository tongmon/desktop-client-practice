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
            color: "#1f0830"

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
                            text: "New Text File"
                        }
                        Action {
                            text: "New File"
                        }
                        Action {
                            text: "New Window"
                        }
                        MenuSeparator {
                            contentItem: Rectangle {
                                implicitHeight: 1
                                color: "#21be2b"
                            }
                            background: Rectangle {
                                color: "#170624"
                            }
                        }
                        Action {
                            text: "Open File"
                        }
                        Action {
                            text: "Open Directory"
                        }

                        delegate: MenuItem {
                            id: menuItem1
                            implicitHeight: titleBar.height

                            contentItem: Text {
                                text: menuItem1.text
                                font: menuItem1.font
                                color: active ? (menuItem1.highlighted ? "#ffffff" : Qt.rgba(0.8, 0.8, 0.8, 1.0)) : Qt.rgba(0.5, 0.5, 0.5, 1.0)
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                anchors.fill: parent
                                opacity: menuItem1.highlighted ? 0.7 : 1.0
                                color: "#170624"
                            }
                        }
                    }
                    Menu {
                        title: "Edit"
                        Action {
                            text: "Undo"
                        }
                        Action {
                            text: "Redo"
                        }
                        Action {
                            text: "Cut"
                        }
                        Action {
                            text: "Copy"
                        }
                        Action {
                            text: "Paste"
                        }

                        delegate: MenuItem {
                            id: menuItem2
                            implicitHeight: titleBar.height

                            contentItem: Text {
                                text: menuItem2.text
                                font: menuItem2.font
                                color: active ? (menuItem2.highlighted ? "#ffffff" : Qt.rgba(0.8, 0.8, 0.8, 1.0)) : Qt.rgba(0.5, 0.5, 0.5, 1.0)
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                anchors.fill: parent
                                opacity: menuItem2.highlighted ? 0.7 : 1.0
                                color: "#170624"
                            }
                        }
                    }
                    Menu {
                        title: "Selected Zone"
                        Action {
                            text: "Select All"
                        }
                        Action {
                            text: "Expand Select Zone"
                        }
                        Action {
                            text: "Collapse Select Zone"
                        }
                        Action {
                            text: "Add Cursor"
                        }
                        Action {
                            text: "Select Line Mode"
                        }

                        delegate: MenuItem {
                            id: menuItem3
                            implicitHeight: titleBar.height

                            contentItem: Text {
                                text: menuItem3.text
                                font: menuItem3.font
                                color: active ? (menuItem3.highlighted ? "#ffffff" : Qt.rgba(0.8, 0.8, 0.8, 1.0)) : Qt.rgba(0.5, 0.5, 0.5, 1.0)
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                anchors.fill: parent
                                opacity: menuItem3.highlighted ? 0.7 : 1.0
                                color: "#170624"
                            }
                        }
                    }
                    Menu {
                        title: "View"
                        Action {
                            text: "Command Pallete"
                        }
                        Action {
                            text: "Open View"
                        }
                        Action {
                            text: "Shape"
                        }
                        Action {
                            text: "Edit Layout"
                        }

                        delegate: MenuItem {
                            id: menuItem4
                            implicitHeight: titleBar.height

                            contentItem: Text {
                                text: menuItem4.text
                                font: menuItem4.font
                                color: active ? (menuItem4.highlighted ? "#ffffff" : Qt.rgba(0.8, 0.8, 0.8, 1.0)) : Qt.rgba(0.5, 0.5, 0.5, 1.0)
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                anchors.fill: parent
                                opacity: menuItem4.highlighted ? 0.7 : 1.0
                                color: "#170624"
                            }
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
            color: "#280a3d"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom
            Button {
                text: "This is Button!"
                anchors.centerIn: parent
            }
        }
    }
}
