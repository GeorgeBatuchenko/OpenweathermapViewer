import QtQuick 2.4

Item {
    width: 70;
    height: 70;

    property string dayPeriod: qsTr("Unknown")
    property string iconFilePath: ""
    property string temperature: qsTr("Unknown")

    Column {
        Text {
            anchors.horizontalCenter: weather.horizontalCenter
            text: dayPeriod
        }
        Row {
            id: weather
            Image {
                id: icon
                source: iconFilePath
            }
            Text {
                anchors.verticalCenter: icon.verticalCenter
                text: temperature
            }
        }
    }
}
