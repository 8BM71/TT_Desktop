import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property alias name: nameLabel.text

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

}
