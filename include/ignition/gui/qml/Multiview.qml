import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2


Window {
    // title: qsTr("Multi-view display")
    id: multiview
    x: 100
    y: 100
    width: 1200
    height: 1000
    minimumWidth: 300
    minimumHeight: 300
    // visible: true

    Shortcut {
    sequence: "Ctrl+F"
    onActivated: showFullScreen()
    }

    Shortcut {
        sequence: "Esc"
        onActivated: show()
    }

    GridLayout {
        anchors.fill: parent
        columns: 2
        rows: 2
        columnSpacing: 0
        rowSpacing: 0

        Rectangle {
            color: "red"
            Layout.column: 0
            Layout.row: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Rectangle {
            color: "green"
            Layout.column: 1
            Layout.row: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Rectangle {
            color: "blue"
            Layout.column: 0
            Layout.row: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Rectangle {
            color: "cyan"
            Layout.column: 1
            Layout.row: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}