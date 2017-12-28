import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property var task: null
    property alias projectName: projectNameLabel.text
    property alias duration: durationLabel.text
    property alias startTime: startTimeLabel.text
    property alias endTime: endTimeLabel.text
    property var contextMenu: null

    signal continueTask()
    signal remove()

    Pane {
        anchors.fill: parent
        Material.elevation: 2
    }

    Label {
        id: taskNameLabel
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
            leftMargin: 10
            right: timeEntriesShowButton.left
            rightMargin: 10
        }
        text: task != null ? task.name : ""

    }

    Button {
        id: timeEntriesShowButton
        anchors {
            right: projectNameLabel.left
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }
        visible: mouseArea.hovered && false //TODO: implement
        width: 40
        height: 40
        text: "5"
        Material.elevation: 0
        Material.background: Material.color(Material.Grey, Material.Shade100)
    }

    Label {
        id: projectNameLabel

        anchors {
            right: timeEntryDataColumn.left
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }
        text: "Project"
    }

    Column {
        id: timeEntryDataColumn

        anchors {
            right: continueButton.left
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }
        width: startStopTimeRow.width

        spacing: 5

        Label {
            id: durationLabel
            anchors.horizontalCenter: parent.horizontalCenter

            verticalAlignment: Label.AlignVCenter
            horizontalAlignment: Label.AlignHCenter

        }

        Row {
            id: startStopTimeRow
            visible: mouseArea.hovered
            spacing: 5

            Label {
                id: startTimeLabel

                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
            }

            Label {
                id: endTimeLabel

                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
            }

        }

    }

    RoundButton {
        id: continueButton
        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
        width: 50
        height: width
        visible: mouseArea.hovered
        Material.elevation: 0

        Image {
            id: continueIcon
            anchors.centerIn: parent
            width: parent.width * 0.6
            height: parent.height * 0.6
            anchors.horizontalCenterOffset: 2
            source: "qrc:/Resources/icons/play-button.svg"
            sourceSize: Qt.size(continueIcon.width, continueIcon.height)
            visible: false
        }
        ColorOverlay {
            anchors.fill: continueIcon
            source: continueIcon
            color: Material.color(settings.accent)
            antialiasing: true
        }
        onClicked: {
            root.continueTask()
        }
    }

    MouseArea {
        id: mouseArea
        property bool hovered: false
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onEntered: {
            hovered = true
        }
        onExited: {
            hovered = false
        }
        propagateComposedEvents: true

        onClicked: {
            mouse.accepted = mouse.button == Qt.RightButton
            if (root.contextMenu)
                root.contextMenu.destroy();
            root.contextMenu = contextMenuComponent.createObject(root, {"x" : mouseX, "y" : mouseY})
        }
        onPressed: {
            mouse.accepted = mouse.button == Qt.RightButton
        }
        onReleased: {
            mouse.accepted = mouse.button == Qt.RightButton
        }
    }

    Component {
        id: contextMenuComponent
        Menu {
            id: contextMenuItem

            MenuItem {
                text: qsTr("Edit") + translator.trString
            }
            MenuItem {
                text: qsTr("Remove") + translator.trString
                onTriggered: root.remove()

            }
            Component.onCompleted: {
                open()
            }
            onClosed: {
                root.contextMenu.destroy()
                root.contextMenu = null
            }
        }
    }
}
