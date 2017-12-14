import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "qrc:/Components"

Item {
    id: root
    objectName: "TimeStoryPage"

    property var taskModel

    property var timeEntryModel

    property var projectModel

    ListView {
        id: timeLineView
        anchors {
            fill: parent
            topMargin: 10
            bottomMargin: 10
        }
        spacing: 10

        model: root.timeEntryModel

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

            property var task: taskModel.getItemData(model.task)

            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 50 /** uiScale.yScale*/

            taskName: task.name
            duration: model.duration
            projectName: projectModel.getItemData(task.project).name
            startTime: model.startTime
            endTime: model.endTime
        }


        ScrollIndicator.vertical: ScrollIndicator { }
    }

}
