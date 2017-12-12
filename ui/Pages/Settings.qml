import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id: root
    objectName: "SettingsPage"

    Column {
        anchors.fill: parent
        anchors.margins: 10 * uiScale.xScale
        spacing: 10 * uiScale.yScale

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Interface")
        }

        Pane {
            width: parent.width
            height: 100 * uiScale.yScale
            anchors.horizontalCenter: parent.horizontalCenter
            Material.elevation: 1

            Row {
                anchors.left: parent.left

                RadioButton {
                    text: qsTr("Light")
                    checked: true
                    onCheckedChanged: {
                        if(checked)
                            appTheme = Material.Light
                    }
                }
                RadioButton {
                    text: qsTr("Dark");
                    onCheckedChanged: {
                        if(checked)
                            appTheme = Material.Dark
                    }
                }
            }

            Row {
                anchors.right: parent.right

                RadioButton {
                    text: qsTr("Orange")
                    onCheckedChanged: {
                        if(checked)
                            appAccent = Material.Orange
                    }
                }
                RadioButton {
                    text: qsTr("Red");

                    onCheckedChanged: {
                        if(checked)
                            appAccent = Material.Red
                    }
                }
                RadioButton {
                    text: qsTr("Blue");
                    onCheckedChanged: {
                        if(checked)
                            appAccent = Material.Blue
                    }
                }
                RadioButton {
                    text: qsTr("Grey");
                    onCheckedChanged: {
                        if(checked)
                            appAccent = Material.Grey
                    }
                }
            }
        }

        Pane {
            width: parent.width
            height: 150 * uiScale.yScale

            anchors.horizontalCenter: parent.horizontalCenter

            Material.elevation: 1

        }

    }

}
