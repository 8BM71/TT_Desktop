import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id: root
    objectName: "StartPage"
    anchors.fill: parent

    signal logined

    Rectangle {
        anchors.fill: parent
    }

    Item {
        id: body
        width: 800 * uiScale.xScale
        height: 600 * uiScale.yScale
        anchors.centerIn: parent

        Pane {
            anchors.fill: parent
            Material.elevation: 6
        }

        Button {
            anchors {
                centerIn: parent
            }

            text: qsTr("LOG IN WITH GOOGLE") + translator.trString

            onClicked: {
                core.siginWithGoogle()
            }
        }

        Connections {
            target: core
            onLogined: {
                root.logined()
            }
        }
    }
}
