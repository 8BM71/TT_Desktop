import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0

Page {
    id: root

    property alias currentIndex: listView.currentIndex
    property string avatar: ""

    header: ToolBar {
        height: 60 /** uiScale.yScale*/
        Material.elevation: 1

        Item {
            anchors.fill: parent
            clip: true

            Image {
                id: userImageIcon
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: height
                anchors.margins: 5 /** uiScale.xScale*/

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

            Column {
                anchors {
                    left: userImageIcon.right
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                    leftMargin: 18
                    topMargin: 10
                    bottomMargin: 10
                }
                spacing: 5

                Label {
                    id: username
                    text: "name"
                }
                Label {
                    id: useremail
                    text: qsTr("email")
                }
            }
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
            width: parent.width
            height: 10 /** uiScale.xScale*/
        }

        model: menuButtonsModel

        footer: ItemDelegate {
            id: footer
            width: parent.width

            Item {
                id: arrowItem

                property int rotationAngle: root.state == "Hidden" ? 0 : 180

                anchors {
                    centerIn: parent
                }
                height: parent.height * 0.6
                width: height

                Image {
                    id: arrowImage
                    anchors.fill: parent
                    source: "qrc:/Resources/icons/double-right-arrows-angles.svg"
                }
                ColorOverlay{
                    anchors.fill: arrowImage
                    source:arrowImage
                    color: Material.color(settings.accent)
                    antialiasing: true
                }

                transform: Rotation{
                    origin.x: arrowItem.width * 0.5
                    origin.y: 0
                    angle: arrowItem.rotationAngle
                    axis {x: 0; y: 1; z:0}
                }

                Behavior on rotationAngle {
                    NumberAnimation {
                        duration: 200
                    }
                }
            }

            MenuSeparator {
                parent: footer
                width: parent.width
                anchors.verticalCenter: parent.top
            }

            onClicked: {
                root.state = root.state == "Normal" ? "Hidden" : "Normal"
            }
        }

        footerPositioning: ListView.OverlayFooter

        delegate: MenuItemDelegate {
            highlighted: index == root.currentIndex
            onClicked: root.currentIndex = index
            title: model.name
        }
        interactive: false

        ScrollIndicator.vertical: ScrollIndicator { }
    }

    state: "Normal"

    states: [
        State {
            name: "Normal"
            PropertyChanges {
                target: root
                width: 200 /** uiScale.xScale*/
            }
        },
        State {
            name: "Hidden"
            PropertyChanges {
                target: root
                width: 60 /** uiScale.xScale*/
            }
        }
    ]

    Behavior on width {
        NumberAnimation {
            duration: 200
        }
    }

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
}
