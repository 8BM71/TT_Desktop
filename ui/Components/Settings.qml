import QtQuick 2.7
import QtQuick.Controls.Material 2.2
import Qt.labs.settings 1.0

Item {
    id: root

    property alias locale: common.locale

    property alias theme: userInterface.theme
    property alias accent: userInterface.accent
    property alias primary: userInterface.primary
    property alias foreground: userInterface.foreground
    property alias background: userInterface.background
    property alias menuState: userInterface.menuState

    Settings {
        id: common
        category: "Common"
        property string locale: "ru_RU"
    }

    Settings {
        id: userInterface
        category: "UI"
        property int theme: Material.Dark
        property int accent: Material.Teal
        property int primary: Material.Grey
        property int foreground: Material.Indigo
        property int background: Material.Grey
        property string menuState: "Normal"

    }
}
