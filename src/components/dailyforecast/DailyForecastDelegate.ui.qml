import QtQuick 2.4

Item {
    anchors.fill: parent

    property string day: qsTr("Unknown")
    property string iconFilePath: ""
    property string temperature: qsTr("Unknown")
    property string clouds: qsTr("Unknown")


    Row {
        Text {
            text: day
        }
        Image {
            id: icon
            source: iconFilePath
        }
        Text {
            text: temperature
        }
        Text {
            color: "#6e5e5e"
            text: clouds
        }
    }
}
