import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "qrc:/Components"

Item {
    id: root
    objectName: "TasksPage"

    property alias model: tasksView.model

    ListView {
        id: tasksView
        anchors {
            fill: parent
            topMargin: 10
            bottomMargin: 10
        }
        spacing: 10 /** uiScale.yScale*/

        section.property: "project"
        section.criteria: ViewSection.FullString
        section.delegate: Item {
            anchors.horizontalCenter: parent.horizontalCenter
            implicitWidth: projectNameLabel.width + 10
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
                id: projectNameLabel
                anchors.centerIn: parent
                text: core.projectsModel.getItemData(section).name
                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
            }
        }

        headerPositioning: ListView.PullBackHeader

        onContentYChanged: {
            if(contentY < -50) {
                if(!core.tasksLoading)
                    core.updateTasksModel();
            }
        }

        header: Item {
            visible: core.tasksLoading

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

        delegate: TaskItemDelegate {
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
                                                                      "taskName": model.name,
                                                                      "taskId": model.itemId
                                                                  })
            }

            onRename: {
                if (editProjectDialog)
                    editProjectDialog.destroy();

                editProjectDialog = editProjectDialogComponent.createObject(root,
                                                                                {
                                                                                    "name": model.name,
                                                                                    "itemId": model.itemId,
                                                                                    "description": model.description
                                                                                })
            }

            onStart: {
                core.startExistTask(model.itemId)
            }

            onSetDefault: {
                core.setProjectAsDefault(model.itemId)
            }
        }

        remove: Transition {
            NumberAnimation { property: "opacity"; to: 0; duration: 400 }
        }

        addDisplaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 400 }
        }

        removeDisplaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 400 }
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }

    property var removeDialog: null
    Component {
        id: removeDialogComponent
        MessageDialog {
            id: removeDialogItem

            property string taskName: ""
            property string taskId: ""

            title: qsTr("Remove task %1").arg(taskName) + translator.trString
            text: qsTr("Do you want to remove this task with all time entries?") + translator.trString

            okButton.text: qsTr("Remove") + translator.trString
            actionButton.visible: false

            onOk: {
                core.deleteTask(removeDialogItem.taskId)
            }

            onAction: {
                core.deleteProject(removeDialogItem.taskId)
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


}
