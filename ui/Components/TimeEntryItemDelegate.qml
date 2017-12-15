import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property alias taskName: taskNameLabel.text
    property alias projectName: projectNameLabel.text
    property alias duration: durationLabel.text
    property var startTime
    property var endTime
    property var contextMenu: null

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
        text: "Taskkk"

    }

    Button {
        id: timeEntriesShowButton
        anchors {
            right: projectNameLabel.left
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }
        visible: mouseArea.hovered
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
            right: resumeButton.left
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }
        width: startStopTimeRow.width

        spacing: 5

        Label {
            id: durationLabel
            anchors.horizontalCenter: parent.horizontalCenter
            text: "10:10:10"

            verticalAlignment: Label.AlignVCenter
            horizontalAlignment: Label.AlignHCenter

        }

        Row {
            id: startStopTimeRow
            visible: mouseArea.hovered
            spacing: 5

            Label {
                id: startTimeLabel
                text: "10:10:10"

                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
            }

            Label {
                id: endTimeLabel
                text: "10:10:10"

                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
            }

        }

    }



    RoundButton {
        id: resumeButton
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
            id: resumeIcon
            anchors.centerIn: parent
            width: parent.width * 0.6
            height: parent.height * 0.6
            anchors.horizontalCenterOffset: 2
            source: "qrc:/Resources/icons/play-button.svg"
            sourceSize: Qt.size(resumeIcon.width, resumeIcon.height)
            visible: false

        }
        ColorOverlay {
            anchors.fill: resumeIcon
            source: resumeIcon
            color: Material.color(settings.accent)
            antialiasing: true
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
                text: qsTr("Edit")
            }
            MenuItem {
                text: qsTr("Remove")
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
