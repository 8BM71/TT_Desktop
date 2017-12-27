import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQml.Models 2.2

import "qrc:/Components"

Item {
    id: root
    objectName: "TimeStoryPage"

    property var taskModel

    property var timeEntryModel

    property var projectModel

    DelegateModel {
        id: visualModel
        model: root.timeEntryModel
    }

    ListView {
        id: timeLineView
        anchors {
            fill: parent
            topMargin: 10
            bottomMargin: 10
        }
        spacing: 10

        model: root.timeEntryModel

        headerPositioning: ListView.PullBackHeader

        onContentYChanged: {
            if(contentY < -50) {
                if(!core.tasksLoading) {
                    core.updateTasksModel();
                    core.updateTimeEntriesModel();
                }
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


        section.property: "startDate"
        section.criteria: ViewSection.FullString
        section.delegate: Item {
            anchors.horizontalCenter: parent.horizontalCenter
            implicitWidth: dateLabel.width + 10
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
                id: dateLabel
                anchors.centerIn: parent
                text: section
                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
            }

        }

        delegate: TimeEntryItemDelegate {

            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 50 /** uiScale.yScale*/

            task: root.taskModel.getItemData(model.task)
            duration: model.duration
            projectName: root.projectModel.getItemData(task.project).name
            startTime: model.startTime
            endTime: model.endTime
            onContinueTask: {
                core.startExistTask(task.itemId)
            }
            onRemove: {
                core.deleteTimeEntry(model.itemId)
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

}
