import QtQuick 2.9
import QtQuick.Controls 2.2

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

        delegate: WorkspaceItemDelegate {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 50 /** uiScale.yScale*/

            name: model.name
            //totalProjects: model.totalProjects

            onRemove: {
                //remove(model.id)
            }
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }
}
