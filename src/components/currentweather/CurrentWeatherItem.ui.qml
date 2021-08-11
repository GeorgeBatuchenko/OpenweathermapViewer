import QtQuick 2.4
import QtGraphicalEffects 1.12

Item {
    width: 450
    height: 190

    Rectangle {
        id: itemRectangle
        color: "#ffffff"
        border.width: 0
        anchors.fill: parent
        anchors.rightMargin: 4
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4

        Column {
            anchors.fill: parent
            anchors.rightMargin: 16
            anchors.leftMargin: 16
            anchors.bottomMargin: 16
            anchors.topMargin: 16

            Text {
                id: fetchDateTime
                color: "#eb6e4b"
                text: "Aug 11, 08:33am"
            }

            Text {
                id: city
                color: "#dd000000"
                text: "London"
                font.bold: true
                font.pointSize: 24
            }

            Row {
                spacing: 16
                Image {
                    id: currentWeather
                    width: 80
                    height: 80
                    source: "qrc:/images/10d@2x.png"
                }
                Text {
                    id: currentTemp
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#dd000000"
                    text: "+15C"
                    font.pointSize: 32
                }
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    Row {
                        spacing: 8
                        Text {
                            color: "#dd000000"
                            text: "Windblow:"
                            font.pointSize: 12
                        }
                        Text {
                            id: windBlow
                            color: "#dd000000"
                            text: "5m/s"
                            font.pointSize: 12
                        }
                    }
                    Row {
                        spacing: 8
                        Text {
                            color: "#dd000000"
                            text: "Humidity:"
                            font.pointSize: 12
                        }
                        Text {
                            id: humidity
                            color: "#dd000000"
                            text: "85%"
                            font.pointSize: 12
                        }
                    }
                }
            }

            Text {
                id: aditionalInfo
                color: "#dd000000"
                text: "Feels like 16°C. Broken clouds. Light breeze."
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                font.bold: true
                font.pointSize: 12
                width: 500
            }
        }
    }

    DropShadow {
        anchors.fill: itemRectangle
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: itemRectangle
    }
}
