import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

ToolBar {
    id: root
    height: 60 /** uiScale.yScale*/
    Material.elevation: 1
    Material.background: Material.Grey

    Connections {
        target: core
        onProjectsModelChanged: {
            if (projectsComboBox.currentIndex == -1 && projectsComboBox.model.count > 0)
                projectsComboBox.currentIndex = 0
        }
        onCurrentTaskChanged: {
            var projId = currentTask.project
            projectsComboBox.currentIndex = projId != undefined ? core.projectsModel.getIndex(projId) : 0
            newTaskName.text = currentTask.name != undefined ? currentTask.name : ""
        }
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

        placeholderText: qsTr("New task...") + translator.trString

        readOnly: core.running

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

        width: 200

        //enabled: !core.running
        textRole: "name"
        model: core.projectsModel

        Material.elevation: 0
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
        font.pixelSize: 18
    }

    RoundButton {
        id: runningButton
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 10
        }

        width: 60
        height: width

        Material.elevation: 0

        Material.background: /*core.waiting ? Material.Yellow : */core.running ? Material.Red : Material.Green

        //text: (core.running ? qsTr("Stop") : qsTr("Start")) + translator.trString
        Image {
            id: runningButtonIcon
            anchors.centerIn: parent
            width: core.running ? parent.width * 0.4 : parent.width * 0.6
            height: width
            anchors.horizontalCenterOffset: core.running ? 0 : 3
            source: core.running ? "qrc:/Resources/icons/stop-button.svg" : "qrc:/Resources/icons/play-button.svg"
            sourceSize: Qt.size(runningButtonIcon.width, runningButtonIcon.height)
            visible: false
        }

        ColorOverlay {
            anchors.fill: runningButtonIcon
            source: runningButtonIcon
            color: "white"
            antialiasing: true
        }

        RotationAnimator {
            id: rotationAnimator
            target: runningButton
            from: 0
            to: 360
            duration: 500
            easing.type: Easing.InOutBack

        }

        Connections {
            target: core
            onRunningChanged: {
                rotationAnimator.start()
            }
        }

        onClicked: {
            if (core.running)
                core.stopTask()
            else
                core.startNewTask(newTaskName.text, core.projectsModel.getItemData(projectsComboBox.currentIndex).itemId)
        }
    }
}

