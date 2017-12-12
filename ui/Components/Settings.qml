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
        property int accent: Material.Teal
        property int primary: Material.BlueGrey
        property int foreground: Material.Brown
        property int background: Material.Grey
    }
}
