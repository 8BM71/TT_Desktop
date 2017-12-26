import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "qrc:/Components"

Item {
    id: root
    objectName: "WorkspacesPage"

    property alias model: workspacesView.model

    ListView {
        id: workspacesView
        anchors {
            fill: parent
            topMargin: 10
            bottomMargin: 10
        }
        spacing: 10 /** uiScale.yScale*/

        headerPositioning: ListView.PullBackHeader

        onContentYChanged: {
            if(contentY < -50) {
                if(!core.workspacesLoading)
                    core.updateWorkspacesModel();
            }
        }

        header: Item {
            visible: core.workspacesLoading

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

        delegate: WorkspaceItemDelegate {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 50 /** uiScale.yScale*/

            name: model.name
            //totalProjects: model.totalProjects

            onRemove: {
                if (removeDialog)
                    removeDialog.destroy();

                removeDialog = removeDialogComponent.createObject(root,
                                                                  {
                                                                      "workspaceName": model.name,
                                                                      "workspaceId": model.itemId
                                                                  })
            }

            onRename: {
                if (renameDialog)
                    renameDialog.destroy();

                renameDialog = renameDialogComponent.createObject(root,
                                                                  {
                                                                      "workspaceName": model.name,
                                                                      "workspaceId": model.itemId
                                                                  })
            }

            onNewWorkspace: {
                if (createDialog)
                    createDialog.destroy();

                createDialog = createDialogComponent.createObject(root)
            }

            onSetDefault: {
                core.setWorkspaceAsDefault(model.itemId)
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

            property string workspaceName: ""
            property string workspaceId: ""

            title: removeDialogItem.workspaceName == "" ? qsTr("Remove workspace ") + translator.trString
                                                        : workspaceName
            text: qsTr("Do you want to remove only workspaceName or all projects in this workspaceName too?") + translator.trString

            okButton.text: qsTr("Remove workspaceName") + translator.trString
            actionButton.text: qsTr("Remove all") + translator.trString

            onOk: {
                core.deleteWorkspace(removeDialogItem.workspaceId)
            }

            onAction: {
                core.deleteWorkspace(removeDialogItem.workspaceId)
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

    property var renameDialog: null
    Component {
        id: renameDialogComponent
        Dialog {
            id: renameDialogItem
            visible: true

            property string workspaceName: ""
            property string workspaceId: ""

            title: qsTr("Rename workspace %1").arg(workspaceName == "" ? "" : workspaceName) + translator.trString

            width: 400
            height: 200

            x: (parent.width - width) * 0.5
            y: (parent.height - height) * 0.5

            Item {
                anchors {
                    fill: parent
                    topMargin: renameDialogItem.header.height
                    bottomMargin: renameDialogItem.footer.height
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

                    }
                }
                ToolButton {
                    id: okButtonItem
                    text: qsTr("Accept") + translator.trString
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

                    onClicked: {
                        core.updateWorkspace(newNameTextField.text, renameDialogItem.workspaceId)
                    }
                }
            }

            onClosed: {
                renameDialog.destroy();
                renameDialog = null;
            }
        }
    }


    property var createDialog: null
    Component {
        id: createDialogComponent
        Dialog {
            id: createDialogItem
            visible: true

            title: qsTr("Create workspace ") + translator.trString

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
                    id: newNameTextField
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.7

                    placeholderText: qsTr("Enter the name...") + translator.trString
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
                        core.createNewWorkspace(newNameTextField.text)
                    }
                }
            }

            onClosed: {
                createDialog.destroy();
                createDialog = null;
            }
        }
    }
}
