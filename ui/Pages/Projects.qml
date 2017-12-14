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

//            onPopupMenu: {
//                if (contextMenu)
//                    contextMenu.destroy();


//                var point = projectDelegateItem.mapToItem(null, mX, mY);
//                console.debug(mX, mY, point.x, point.y)

//                contextMenu = contextMenuComponent.createObject(root,
//                                                                {
//                                                                    "x": point.x,
//                                                                    "y": point.y,
//                                                                    "projectName": model.name,
//                                                                    "projectId": model.itemId
//                                                                })
//            }
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

}
