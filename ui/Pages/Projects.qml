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
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: 50 /** uiScale.yScale*/

            name: model.name
            //totalTime: model.totalTime

            onRemove: {
                //remove(model.id)
            }
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }

}
