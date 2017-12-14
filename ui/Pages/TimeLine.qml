import QtQuick 2.9
import QtQuick.Controls 2.2

import "qrc:/Components"

Item {
    id: root
    objectName: "TimeLinePage"

    property var taskModel

    property var timeEntryModel

    property alias model: timeLineView.model

    ListView {
        id: timeLineView
        anchors {
            fill: parent
            topMargin: 10
            bottomMargin: 10
        }
        spacing: 10

        delegate: TimeEntryItemDelegate {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 50 /** uiScale.yScale*/

            name: model.startDate//model.name
        }


        ScrollIndicator.vertical: ScrollIndicator { }
    }

}
