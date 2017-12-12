import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3

import "qrc:/Pages"
import "qrc:/Components"

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: qsTr("tpu-tt")

    property int appTheme: Material.Light
    property int appAccent: Material.LightBlue

    property bool logined: false

    Material.theme: appTheme
    Material.accent: appAccent

    property Scale uiScale: Scale {
        xScale: window.width / 1920
        yScale: window.height / 1080
    }

    Drawer {
        id: drawer
        width: 0.66 * window.width
        height: window.height
    }


    Navigator {
        stackView: bodyItem
        currentPage: menuBar.currentIndex
    }

    Page {
        anchors {
            left: menuBar.right
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        StackView {
            id: bodyItem
            anchors.fill: parent

            pushEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to:1
                    duration: 200
                }
            }
            pushExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to:0
                    duration: 200
                }
            }
            popEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to:1
                    duration: 200
                }
            }
            popExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to:0
                    duration: 200
                }
            }
        }

        header: ToolBar {
            height: 150 * uiScale.yScale
            Material.accent: Material.Orange
            Material.background: Material.Teal
            Material.elevation: 1
        }
    }

    MenuBar {
        id: menuBar
        width: 400 * uiScale.xScale
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }

    }

    property var startPage: null
    Component {
        id: startPageComponent
        Start {
            Label {
                anchors.centerIn: parent
                text: parent.objectName
            }
            onLogined: {
                if(startPage)
                    startPage.destroy()
            }
        }
    }

    Component.onCompleted: {
        if(!logined) {
            if(startPage)
                startPage.destroy()

            startPage = startPageComponent.createObject(window)
        }
    }

}
