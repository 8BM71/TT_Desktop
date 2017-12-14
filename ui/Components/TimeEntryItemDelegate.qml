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
            right: projectNameLabel.left
            rightMargin: 10
        }

    }

    Label {
        id: projectNameLabel

        anchors {
            right: durationLabel.left
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }


    }

    Label {
        id: durationLabel

        anchors {
            right: resumeButton.left
            verticalCenter: parent.verticalCenter
            rightMargin: 10
        }

        verticalAlignment: Label.AlignVCenter

    }

    RoundButton {
        id: resumeButton
        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
        width: 40
        height: width

//        Image {
//            id: resumeIcon
//            source: "file"
//        }
//        ColorOverlay {

//        }
    }

    MouseArea {

    }
}
