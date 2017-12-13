import QtQuick 2.9
import QtQuick.Controls 2.2

import "qrc:/Pages"

Item {
    id: root
    property StackView stackView: null
    property int currentPage: -1

    function popByName(name) {
        var findedItem = root.stackView.find(function(item, index) {
            return item.objectName === name;
        });

        if(findedItem) {
            stackView.pop(findedItem);
            return true;
        } else
            return false;
    }

    onCurrentPageChanged: {
        switch(root.currentPage) {
        case 0:
            if(!popByName("TimeLinePage"))
                root.stackView.push(timeLinePageComponent)
            break
        case 1:
            if(!popByName("WorkspacesPage"))
                root.stackView.push(workspacesPageComponent)
            break
        case 2:
            if(!popByName("ProjectPage"))
                root.stackView.push(projectsPageComponent)
            break
        case 3:
            if(!popByName("TasksPage"))
                root.stackView.push(tasksPageComponent)
            break
        case 4:
            if(!popByName("SettingsPage"))
                root.stackView.push(settingsPageComponent)
            break
        default:
            if(!popByName("TimeLinePage"))
                root.stackView.push(timeLinePageComponent)
            break
        }
    }

    Component {
        id: timeLinePageComponent
        TimeLine {
            Label {
                anchors.centerIn: parent
                text: parent.objectName
            }
        }
    }

    Component {
        id: workspacesPageComponent
        Workspaces {
            model: workspacesModel
        }
    }

    Component {
        id: projectsPageComponent
        Projects {
            model: projectModel
        }
    }

    Component {
        id: tasksPageComponent
        Tasks {
            Label {
                anchors.centerIn: parent
                text: parent.objectName
            }
        }
    }
    Component {
        id: settingsPageComponent
        Settings {
            Label {
                anchors.centerIn: parent
                text: parent.objectName
            }
        }
    }
}
