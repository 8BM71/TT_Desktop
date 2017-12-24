import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Dialog {
    id: root
    visible: true

    property alias text: bodyText.text
    property alias okButton: okButtonItem
    property alias cancelButton: cancelButtonItem
    property alias actionButton: actionButtonItem

    signal action
    signal cancel
    signal ok

    width: 400
    height: 200

    x: (parent.width - width) * 0.5
    y: (parent.height - height) * 0.5

    title: qsTr("Message title") + translator.trString

    Label {
        id: bodyText
        anchors {
            fill: parent
            topMargin: root.header.height
            bottomMargin: root.footer.height
        }


        clip: true
        elide: Label.ElideRight
        wrapMode: Label.WordWrap

        font.pointSize: 12

        text: qsTr("Message text") + translator.trString
    }

    footer: DialogButtonBox {
        ToolButton {
            id: cancelButtonItem
            text: qsTr("Cancel") + translator.trString
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole

            onClicked: {
                root.cancel()
            }
        }
        ToolButton {
            id: actionButtonItem
            text: qsTr("Action") + translator.trString
            DialogButtonBox.buttonRole: DialogButtonBox.ActionRole

            onClicked: {
                root.action()
            }
        }
        ToolButton {
            id: okButtonItem
            text: qsTr("Ok") + translator.trString
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

            onClicked: {
                root.ok()
            }
        }
    }
}
