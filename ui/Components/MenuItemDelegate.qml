import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

MenuItem {
    width: parent.width

    property alias title: label.text

    clip: true

    Item {
        id: icon
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
            leftMargin: 15
            topMargin: 10
            bottomMargin: 10

        }
        width: height

        Image {
            id: iconImage
            anchors.fill: icon
            visible: false

            source: model.icon
            sourceSize: Qt.size(iconImage.width, iconImage.height)
        }
        ColorOverlay{
            anchors.fill: iconImage
            source:iconImage
            color: Material.color(settings.accent)
            antialiasing: true
        }

        Label {
            id: label
            anchors {
                left: icon.right
                right: parent.right
                verticalCenter: parent.verticalCenter
                leftMargin: 18
            }
        }
    }

}
