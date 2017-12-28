import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3


import "qrc:/Pages"
import "qrc:/Components"

ApplicationWindow {
    id: window
    visible: true

    minimumWidth: 600
    minimumHeight: 450
    width: 800
    height: 600

    title: Application.name

    property bool logined: false//true
    property alias settings: settingsItem

    Material.theme: settings.theme
    Material.accent: settings.accent
    Material.primary: settings.primary
    Material.foreground: settings.foreground
    //Material.background: settings.background

    property Scale uiScale: Scale {
        xScale: window.width / 800
        yScale: window.height / 600
    }

    Settings {
        id: settingsItem

        onLocaleChanged: translator.locale = settingsItem.locale
        Component.onCompleted: translator.locale = settingsItem.locale
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

        header: TimerLine {

        }
    }

    MenuBar {
        id: menuBar
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
//            Label {
//                anchors.centerIn: parent
//                text: parent.objectName
//            }
            onLogined: {
                if(startPage)
                    startPage.destroy()
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+L"
        onActivated: {
            core.siginWithGoogle()
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
