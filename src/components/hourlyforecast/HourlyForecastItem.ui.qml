import QtQuick 2.4
import QtGraphicalEffects 1.12

Item {
    width: 450
    height: 135

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
            spacing: 8

            Text {
                color: "#dd000000"
                text: qsTr("Hourly forecast")
                font.bold: true
                font.pointSize: 14
            }

            Row {
                spacing:70
                HourlyForecastDelegate {
                    id: hourlyForecast1
                    dayPeriod: "4:00am"
                    iconFilePath: "qrc:/images/10d.png"
                    temperature: "+8C"
                }
                HourlyForecastDelegate {
                    id: hourlyForecast2
                    dayPeriod: "12:00am"
                    iconFilePath: "qrc:/images/10d.png"
                    temperature: "+15C"
                }
                HourlyForecastDelegate {
                    id: hourlyForecast3
                    dayPeriod: "8:00pm"
                    iconFilePath: "qrc:/images/10d.png"
                    temperature: "+13C"
                }
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
