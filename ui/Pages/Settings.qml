import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Page {
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
                            settings.theme = Material.Light
                    }
                }
                RadioButton {
                    text: qsTr("Dark");
                    onCheckedChanged: {
                        if(checked)
                            settings.theme = Material.Dark
                    }
                }
            }

            Row {
                anchors.right: parent.right

                RadioButton {
                    text: qsTr("Orange")
                    onCheckedChanged: {
                        if(checked)
                            settings.accent = Material.Orange
                    }
                }
                RadioButton {
                    text: qsTr("Red");

                    onCheckedChanged: {
                        if(checked)
                            settings.accent = Material.Red
                    }
                }
                RadioButton {
                    text: qsTr("Blue");
                    onCheckedChanged: {
                        if(checked)
                            settings.accent = Material.Blue
                    }
                }
                RadioButton {
                    text: qsTr("Grey");
                    onCheckedChanged: {
                        if(checked)
                            settings.accent = Material.Grey
                    }
                }
            }
        }

        Pane {
            width: parent.width
            height: 150 * uiScale.yScale

            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                anchors{
                    left: parent.left
                    top: parent.top
                    right: parent.right
                }

                height: 50
                spacing: 10

                Label {
                    text: qsTr("Language") + translator.trString
                    anchors.verticalCenter: parent.verticalCenter
                }

                ComboBox {
                    id: langCB
                    anchors.verticalCenter: parent.verticalCenter
                    model: translator.translationModel
                    textRole: "locale"

                    onActivated: {
                        settings.locale = langCB.textAt(index);
                    }

                    Material.elevation: 0
                }
            }

            Material.elevation: 1

        }

    }

}
