import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property alias name: nameLabel.text
    property string totalProjects

    signal remove

    Pane {
        anchors.fill: parent
        Material.elevation: 2
    }

    Label {
        id: nameLabel

        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
            margins: 10
        }

        verticalAlignment: Label.AlignVCenter
        clip: true
        elide: Label.ElideRight

        width: parent.width * 0.6
    }


    Label {
        id: totalProjectsLabel

        anchors {
            top: parent.top
            left: nameLabel.right
            bottom: parent.bottom
            margins: 10
        }

        verticalAlignment: Label.AlignVCenter
        clip: true
        elide: Label.ElideRight

        text: root.totalTime + " " + qsTr("h")

        width: parent.width * 0.2
    }

    RoundButton {
        id: removeButton
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
            //margins: 3 /** uiScale.xScale*/
            rightMargin: 15 /** uiScale.xScale*/
        }
        width: height

        Material.elevation: 0

        Image {
            id: removeIcon
            anchors.centerIn: parent
            width: parent.width * 0.5
            height: parent.height * 0.5
            source: "qrc:/Resources/icons/recycling-bin.svg"
            sourceSize: Qt.size(removeIcon.width, removeIcon.height)
            visible: false
        }
        ColorOverlay{
            anchors.fill: removeIcon
            source: removeIcon
            color: Material.color(settings.accent)
            antialiasing: true
        }

        onClicked: {
            root.remove()
        }
    }
}
