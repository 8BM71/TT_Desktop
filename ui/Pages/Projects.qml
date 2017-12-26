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

        headerPositioning: ListView.PullBackHeader

        onContentYChanged: {
            if(contentY < -50) {
                if(!core.projectsLoading)
                    core.updateProjectsModel();
            }
        }

        header: Item {
            visible: core.projectsLoading

            height: visible ? 50 : 0
            width: parent.width

            BusyIndicator {
                anchors.centerIn: parent
                height: 50 * 0.8
                width: height
                running: true
            }

            Behavior on height {
                NumberAnimation { duration: 250 }
            }
        }

        delegate: ProjectItemDelegate {
            id: projectDelegateItem
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 50 /** uiScale.yScale*/

            name: model.name
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
                if (createDialog)
                    createDialog.destroy();

                createDialog = createDialogComponent.createObject(root)
            }

            onSetDefault: {
                core.setProjectAsDefault(model.itemId)
            }
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }

    RoundButton {
        text: qsTr("+") + translator.trString
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
            if (createDialog)
                createDialog.destroy();

            createDialog = createDialogComponent.createObject(root)
        }
    }

    property var removeDialog: null
    Component {
        id: removeDialogComponent
        MessageDialog {
            id: removeDialogItem

            property string projectName: ""
            property string projectId: ""

            title: removeDialogItem.projectName == "" ? qsTr("Remove project ") + translator.trString
                                                      : projectName
            text: qsTr("Do you want to remove only project or all tasks in this projects too?") + translator.trString

            okButton.text: qsTr("Remove project") + translator.trString
            actionButton.text: qsTr("Remove all") + translator.trString

            onOk: {
                core.deleteProject(removeDialogItem.projectId)
            }

            onAction: {
                core.deleteProject(removeDialogItem.projectId)
                //TODO: implement
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


    property var createDialog: null
    Component {
        id: createDialogComponent
        Dialog {
            id: createDialogItem
            visible: true

            property string projectName: ""
            property string projectId: ""

            title: qsTr("New project") + translator.trString

            width: 400
            height: 200

            x: (parent.width - width) * 0.5
            y: (parent.height - height) * 0.5

            Item {
                anchors {
                    fill: parent
                    topMargin: createDialogItem.header.height
                    bottomMargin: createDialogItem.footer.height
                }

                TextField {
                    id: createTextField
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.5
                    placeholderText: qsTr("Enter the name..") + translator.trString
                }

                ComboBox {
                    id: workspacesComboBox
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: createTextField.right
                        right: parent.right
                    }

                    textRole: "name"
                    model: core.workspacesModel
                }
            }


            footer: DialogButtonBox {
                ToolButton {
                    id: cancelButtonItem
                    text: qsTr("Cancel") + translator.trString
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole

                    onClicked: {

                    }
                }
                ToolButton {
                    id: okButtonItem
                    text: qsTr("Accept") + translator.trString
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

                    onClicked: {
                        core.createNewProject(createTextField.text, core.workspacesModel.get(workspacesComboBox.currentIndex).id)
                    }
                }
            }

            onClosed: {
                createDialog.destroy();
                createDialog = null;
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

            title: qsTr("Rename project %1").arg(projectName == "" ? "" : projectName) + translator.trString

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
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.7

                    placeholderText: qsTr("New name...") + translator.trString
                }
            }

            footer: DialogButtonBox {
                ToolButton {
                    id: cancelButtonItem
                    text: qsTr("Cancel") + translator.trString
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole

                    onClicked: {
                        //root.cancel()
                    }
                }
                ToolButton {
                    id: okButtonItem
                    text: qsTr("Accept") + translator.trString
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

                    onClicked: {
                        core.updateProject(newNameTextField.text, renameProjectDialogItem.projectId)
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
