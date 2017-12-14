import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "qrc:/Components"

Item {
    id: root
    objectName: "ProjectPage"

    property alias model: projectsView.model

    ListView {
        id: projectsView
        anchors {
            fill: parent
            topMargin: 10
            bottomMargin: 10
        }
        spacing: 10 /** uiScale.yScale*/

        section.property: "workspace"
        section.criteria: ViewSection.FullString
        section.delegate: Item {
            anchors.horizontalCenter: parent.horizontalCenter
            implicitWidth: workspaceNameLabel.width + 10
            height: 50

            Pane {
                anchors {
                    fill: parent
                    topMargin: 10
                    bottomMargin: 10
                }
                Material.elevation: 2
            }

            Label {
                id: workspaceNameLabel
                anchors.centerIn: parent
                text: core.workspacesModel.getItemData(section).name
                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
            }
        }

        delegate: ProjectItemDelegate {
            id: projectDelegateItem
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 50 /** uiScale.yScale*/

            name: model.name
            projectId: model.itemId
            //totalTime: model.totalTime

            onRemove: {
                if (removeDialog)
                    removeDialog.destroy();

                removeDialog = removeDialogComponent.createObject(root,
                                                                  {
                                                                      "projectName": model.name,
                                                                      "projectId": model.itemId
                                                                  })
            }

            onRename: {
                if (renameProjectDialog)
                    renameProjectDialog.destroy();

                renameProjectDialog = renameProjectDialogComponent.createObject(root,
                                                                                {
                                                                                    "projectName": model.name,
                                                                                    "projectId": model.itemId
                                                                                })
            }

            onNewProject: {
                if (newProjectDialog)
                    newProjectDialog.destroy();

                newProjectDialog = newProjectDialogComponent.createObject(root)
            }
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }

    RoundButton {
        text: qsTr("+")
        width: 70
        height: width
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 25
        }
        font.pointSize: 16
        highlighted: true
        onClicked: {
            if (newProjectDialog)
                newProjectDialog.destroy();

            newProjectDialog = newProjectDialogComponent.createObject(root)
        }
    }

    property var removeDialog: null
    Component {
        id: removeDialogComponent
        MessageDialog {
            id: removeDialogItem

            property string projectName: ""
            property string projectId: ""

            title: removeDialogItem.projectName == "" ? qsTr("Remove project ")
                                                      : projectName
            text: qsTr("Do you want to remove only project or all tasks in this projects too?")

            okButton.text: qsTr("Remove project")
            actionButton.text: qsTr("Remove all")

            onOk: {
                core.deleteProject(removeDialogItem.projectId)
            }

            onAction: {
                core.deleteProject(removeDialogItem.projectId)
                removeDialogItem.close();
            }

            onCancel: {

            }

            onClosed: {
                removeDialog.destroy();
                removeDialog = null;
            }
        }
    }


    property var newProjectDialog: null
    Component {
        id: newProjectDialogComponent
        Dialog {
            id: newProjectDialogItem
            visible: true

            property string projectName: ""
            property string projectId: ""

            title: qsTr("New project")

            width: 400
            height: 200

            x: (parent.width - width) * 0.5
            y: (parent.height - height) * 0.5

            Item {
                anchors {
                    fill: parent
                    topMargin: newProjectDialogItem.header.height
                    bottomMargin: newProjectDialogItem.footer.height
                }

                TextField {
                    id: newProjectTextField
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.5
                    text: newProjectDialogItem.projectName
                    placeholderText: qsTr("Enter the name..")
                }

                ComboBox {
                    id: workspacesComboBox
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: newProjectTextField.right
                        right: parent.right
                    }

                    textRole: "name"
                    model: core.workspacesModel
                }
            }


            footer: DialogButtonBox {
                ToolButton {
                    id: cancelButtonItem
                    text: qsTr("Cancel")
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole

                    onClicked: {
                        //root.cancel()
                    }
                }
                ToolButton {
                    id: okButtonItem
                    text: qsTr("Accept")
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

                    onClicked: {
                        //root.ok()
                    }
                }
            }

            onClosed: {
                newProjectDialog.destroy();
                newProjectDialog = null;
            }
        }
    }

    property var renameProjectDialog: null
    Component {
        id: renameProjectDialogComponent
        Dialog {
            id: renameProjectDialogItem
            visible: true

            property string projectName: ""
            property string projectId: ""

            title: renameProjectDialogItem.projectName == "" ? qsTr("Rename project ")
                                                             : projectName

            width: 400
            height: 200

            x: (parent.width - width) * 0.5
            y: (parent.height - height) * 0.5

            Item {
                anchors {
                    fill: parent
                    topMargin: renameProjectDialogItem.header.height
                    bottomMargin: renameProjectDialogItem.footer.height
                }

                TextField {
                    id: newNameTextField
                    text: renameProjectDialogItem.projectName
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.7

                    placeholderText: qsTr("New name...")
                }
            }

            footer: DialogButtonBox {
                ToolButton {
                    id: cancelButtonItem
                    text: qsTr("Cancel")
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole

                    onClicked: {
                        //root.cancel()
                    }
                }
                ToolButton {
                    id: okButtonItem
                    text: qsTr("Accept")
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

                    onClicked: {
                        //root.ok()
                    }
                }
            }

            onClosed: {
                renameProjectDialog.destroy();
                renameProjectDialog = null;
            }
        }
    }

}
