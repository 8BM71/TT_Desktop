import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

Page {
    id: root

    property alias currentIndex: listView.currentIndex
    property string avatar: ""

    ListModel {
        id: menuButtonsModel
        ListElement {
            name: qsTr("History")
            icon: "qrc:/Resources/icons/timer-timing-tool.svg"
        }
        ListElement {
            name: qsTr("Workspaces")
            icon: "qrc:/Resources/icons/cube-of-notes-stack.svg"
        }
        ListElement {
            name: qsTr("Projects")
            icon: "qrc:/Resources/icons/folder-outline.svg"
        }
        ListElement {
            name: qsTr("Tasks")
            icon: "qrc:/Resources/icons/clipboard-square-symbol.svg"
        }
        ListElement {
            name: qsTr("Settings")
            icon: "qrc:/Resources/icons/gear-outlined-symbol.svg"
        }
    }

    header: ToolBar {
        height: 150 * uiScale.yScale
        Material.accent: Material.Orange
        Material.background: Material.Teal
        Material.elevation: 1

        Image {
            id: userImageIcon
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            anchors.margins: 10 * uiScale.xScale

            visible: false

            source: "qrc:/Resources/icons/images-square-outlined-interface-button-symbol.svg"
            sourceSize: Qt.size(userImage.width, userImage.height)
        }

        ColorOverlay{
            visible: userImage.status != Image.Ready
            anchors.fill: userImageIcon
            source: userImageIcon
            color: Material.color(settings.accent)
            antialiasing: true
        }

        Image {
            id: userImage
            anchors.fill: userImageIcon
            fillMode: Image.PreserveAspectFit
            source: root.avatar
        }
    }

    Pane {
        anchors.fill: parent
        Material.elevation: 3
    }


    ListView {
        id: listView
        anchors.fill: parent
        spacing: 2 * uiScale.yScale

        header: ItemDelegate {
            id: header
            //text: qsTr("Header")
            width: parent.width
            height: 10 * uiScale.xScale

            //MenuSeparator {
            //    parent: header
            //    width: parent.width
            //    anchors.verticalCenter: parent.bottom
            //}
        }

        model: menuButtonsModel

        footer: ItemDelegate {
            id: footer
            text: qsTr("Footer")
            width: parent.width

            MenuSeparator {
                parent: footer
                width: parent.width
                anchors.verticalCenter: parent.top
            }
        }

        footerPositioning: ListView.OverlayFooter

        delegate: menuItemDelegate
        interactive: false

        ScrollIndicator.vertical: ScrollIndicator { }
    }

    Component {
        id: menuItemDelegate
        MenuItem {
            text: model.name
            width: parent.width
            highlighted: index == root.currentIndex
            onClicked: root.currentIndex = index

            leftPadding: iconImage.width + 30 * uiScale.xScale

            Image {
                id: iconImage
                anchors {
                    top: parent.top
                    left: parent.left
                    bottom: parent.bottom
                    margins: 10 * uiScale.yScale
                }
                width: height
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
        }
    }


}
