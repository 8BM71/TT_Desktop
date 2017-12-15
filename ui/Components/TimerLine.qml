import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

ToolBar {
    id: root
    height: 60 /** uiScale.yScale*/
    Material.elevation: 1
    Material.background: Material.Grey

    QtObject {
        id: internal
        property bool running: false
    }

    TextField {
        id: newTaskName
        anchors {
            left: parent.left
            leftMargin: 10
            right: projectsComboBox.left
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }

        placeholderText: qsTr("New task...")

        readOnly: internal.running

        width: parent.width * 0.5
        font.pixelSize: 18
    }

    ComboBox {
        id: projectsComboBox
        anchors {
            right: durationLabel.left
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }

        width: 150

        Material.elevation: 0


        textRole: "name"
        model: core.projectsModel
    }

    Label {
        id: durationLabel
        anchors {
            right: runningButton.left
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
        verticalAlignment: Label.AlignVCenter
        horizontalAlignment: Label.AlignHCenter

        text: core.timerDuration//"00.00.00"
        font.pixelSize: 16
    }

    Button {
        id: runningButton
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 10
        }

        width: 80

        Material.elevation: 0

        Material.background: internal.running ? Material.Red : Material.Green

        text: internal.running ? qsTr("Stop") : qsTr("Start")
        onClicked: {
            core.startNewTask(newTaskName.text, projectsComboBox.currentIndex)
            internal.running = !internal.running
        }
    }
}

