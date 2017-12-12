import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Page {
    id: root

    property alias currentIndex: listView.currentIndex

    ListModel {
        id: menuButtonsModel
        ListElement {
            name: qsTr("History")
            icon: ""
        }
        ListElement {
            name: qsTr("Workspaces")
            icon: ""
        }
        ListElement {
            name: qsTr("Projects")
            icon: ""
        }
        ListElement {
            name: qsTr("Tasks")
            icon: ""
        }
        ListElement {
            name: qsTr("Settings")
            icon: ""
        }
    }

    header: ToolBar {
        height: 150 * uiScale.yScale
        Material.accent: Material.Orange
        Material.background: Material.Teal
        Material.elevation: 1

        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            border.width: 2
            anchors.margins: 10 * uiScale.xScale
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
        }
    }


}
