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
                if (editDialog)
                    editDialog.destroy();

                editDialog = editDialogComponent.createObject(root,
                                                              {
                                                                  "name": model.name,
                                                                  "itemId": model.itemId,
                                                                  "description": model.description,
                                                                  "project": model.project
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

    property var editDialog: null
    Component {
        id: editDialogComponent
        MessageDialog {
            id: editDialogItem

            width: 500
            height: 300

            property string name: ""
            property string itemId: ""
            property string description: ""
            property string project: ""

            title: qsTr("Edit task %1").arg(name) + translator.trString
            //text: qsTr("Do you want to remove this task with all time entries?") + translator.trString
            text: ""

            Item {
                anchors {
                    fill: parent
                    topMargin: editDialogItem.header.height
                    bottomMargin: editDialogItem.footer.height
                }

                Row {
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: descriptionItem.top
                        bottomMargin: 5
                    }

                    spacing: 5

                    TextField {
                        id: createTextField
                        text: editDialogItem.name
                        placeholderText: qsTr("Enter the name..") + translator.trString
                        width: (parent.width  - parent.spacing) * 0.5
                        selectByMouse: true
                    }

                    ComboBox {
                        id: projectsComboBox
                        width: (parent.width  - parent.spacing) * 0.5

                        textRole: "name"
                        model: core.projectsModel
                        currentIndex: core.projectsModel.getIndex(editDialogItem.project)
                    }
                }

                Flickable {
                    id: descriptionItem
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        bottomMargin: -10
                    }
                    height: parent.height * 0.6
                    clip: true

                    TextArea.flickable: TextArea {
                        id: descriptionTextArea
                        wrapMode: TextArea.WordWrap
                        selectByMouse: true
                        selectByKeyboard: true

                        text: editDialogItem.description

                        placeholderText: qsTr("Task description..") + translator.trString

                    }
                    ScrollIndicator.vertical: ScrollIndicator { }
                }
            }

            okButton.text: qsTr("Accept") + translator.trString
            actionButton.visible: false

            onOk: {
                core.updateTask(editDialogItem.itemId,
                                createTextField.text,
                                core.projectsModel.getItemData(projectsComboBox.currentIndex).itemId,
                                descriptionTextArea.text)
            }

            onCancel: {

            }

            onClosed: {
                editDialog.destroy();
                editDialog = null;
            }
        }
    }




}
