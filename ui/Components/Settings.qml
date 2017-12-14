import QtQuick 2.7
import QtQuick.Controls.Material 2.2
import Qt.labs.settings 1.0

Item {
    id: root

    property alias theme: userInterface.theme
    property alias accent: userInterface.accent
    property alias primary: userInterface.primary
    property alias foreground: userInterface.foreground
    property alias background: userInterface.background

    Settings {
        id: userInterface
        property int theme: Material.Dark
        property int accent: Material.DeepOrange
        property int primary: Material.Red
        property int foreground: Material.Indigo
        property int background: Material.Grey
    }
}
