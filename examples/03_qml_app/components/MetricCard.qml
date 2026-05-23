import QtQuick
import QtQuick.Controls

Rectangle {
    required property string title
    required property string value

    width: 180
    height: 120
    radius: 8
    color: "#f5f7fb"
    border.color: "#c7d0dd"

    Column {
        anchors.centerIn: parent
        spacing: 8

        Label {
            text: value
            font.pixelSize: 34
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label {
            text: title
            color: "#4f5b67"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
