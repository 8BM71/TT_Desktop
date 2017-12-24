import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

ToolBar {
    id: root
    height: 60 /** uiScale.yScale*/
    Material.elevation: 1
    Material.background: Material.Grey

    TextField {
        id: newTaskName
        anchors {
            left: parent.left
            leftMargin: 10
            right: projectsComboBox.left
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }

        placeholderText: qsTr("New task...") + translator.trString

        readOnly: core.running

        width: parent.width * 0.5
        font.pixelSize: 18
    }

    Connections {
        target: core
        onRunningChanged: {
            if (!core.running)
                newTaskName.clear()
        }
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

        Connections {
            target: core
            onProjectsModelChanged: {
                if (projectsComboBox.currentIndex == -1 && projectsComboBox.model.count > 0)
                    projectsComboBox.currentIndex = 0
            }
        }
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

        text: core.timerDuration
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

        Material.background: core.waiting ? Material.Yellow : core.running ? Material.Red : Material.Green

        text: (core.running ? qsTr("Stop") : qsTr("Start")) + translator.trString
        onClicked: {
            if (core.running)
                core.stopTask()
            else
                core.startNewTask(newTaskName.text, core.projectsModel.getItemData(projectsComboBox.currentIndex).itemId)
        }
    }
}

