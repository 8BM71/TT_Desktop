import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

Item {
    id: root

    property alias name: nameLabel.text
    property string totalProjects: ""

    signal remove
    signal newWorkspace
    signal rename
    signal setDefault

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
        visible: root.totalTime != "" && root.totalTime != undefined

        anchors {
            top: parent.top
            left: nameLabel.right
            bottom: parent.bottom
            margins: 10
        }

        verticalAlignment: Label.AlignVCenter
        clip: true
        elide: Label.ElideRight

        text: qsTr("%1 h").arg(root.totalTime) + translator.trString

        width: parent.width * 0.2
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {

            if (contextMenu)
                contextMenu.destroy();

            contextMenu = contextMenuComponent.createObject(root,
                                                            {
                                                                "x": mouseX,
                                                                "y": mouseY,
                                                                "workspaceName": model.name,
                                                                "workspaceId": model.itemId
                                                            })
        }
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

    property var contextMenu: null
    Component {
        id: contextMenuComponent
        Menu {
            id: contextMenuItem

            property string workspaceName: ""
            property string workspaceId: ""

            MenuItem {
                text: qsTr("New") + translator.trString
                onTriggered: {
                    root.newWorkspace()
                }
            }
            MenuItem {
                text: qsTr("Rename") + translator.trString
                onTriggered: {
                    root.rename()
                }
            }
            MenuItem {
                text: qsTr("Remove") + translator.trString
                onTriggered: {
                    root.remove()
                }
            }
            MenuItem {
                text: qsTr("Set default") + translator.trString
                onTriggered: {
                    root.setDefault()
                }
            }
            Component.onCompleted: {
                open()
            }

            onClosed: {
                contextMenu.destroy()
                contextMenu = null
            }
        }
    }
}
