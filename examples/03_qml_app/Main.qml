import QtQuick
import QtQuick.Controls
import Study.Dashboard

ApplicationWindow {
    width: 900
    height: 560
    visible: true
    title: "QML Dashboard"

    Row {
        anchors.centerIn: parent
        spacing: 16

        MetricCard {
            title: "Builds"
            value: "24"
        }

        MetricCard {
            title: "Targets"
            value: "7"
        }
    }
}
