import QtQuick 2.4
import QtQml 2.11
import QtGraphicalEffects 1.0

import OpenWeatherMapViewer 1.0

Item {
    width: 450
    height: 190

    property var model
    //Component.onCompleted: model.update()

    Rectangle {
        id: itemRectangle
        color: "#ffffff"
        border.width: 0
        anchors.fill: parent
        anchors.rightMargin: 4
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4

        function temperatureToText(temp) {
            if(temp > 0) {
                return qsTr("+") + Number(temp).toLocaleString(Qt.locale(),"f", 0) + qsTr("℃")
            } if(temp < 0) {
                return qsTr("-") + Number(temp).toLocaleString(Qt.locale(),"f", 0) + qsTr("℃")
            } else {
                return Number(temp).toLocaleString(Qt.locale(),"f", 0) + qsTr("℃")
            }
        }

        function humadityToText(humidity) {
                return Number(humidity).toLocaleString(Qt.locale(),"f", 0) + qsTr("%")
        }

        Column {
            anchors.fill: parent
            anchors.rightMargin: 16
            anchors.leftMargin: 16
            anchors.bottomMargin: 16
            anchors.topMargin: 16

            Text {
                id: fetchDateTime
                color: "#eb6e4b"
                font.family: "Arial"
                font.pointSize: 12
                text: model.fetchingDateTime
            }

            Text {
                id: city
                color: "#dd000000"
                font.family: "Arial"
                text:  model.city
                font.bold: true
                font.pointSize: 24
            }

            Row {
                spacing: 16
                Image {
                    id: currentWeather
                    width: 80
                    height: 80
                    source: model.iconFilePath
                }
                Text {
                    id: currentTemp
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#dd000000"
                    font.family: "Arial"
                    text: itemRectangle.temperatureToText(model.temperature)
                    font.pointSize: 32
                }
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    Row {
                        spacing: 8
                        Text {
                            color: "#dd000000"
                            text: "Windblow:"
                            font.family: "Arial"
                            font.pointSize: 12
                        }
                        Text {
                            id: windBlow
                            color: "#dd000000"
                            font.family: "Arial"
                            text: model.windblow
                            font.pointSize: 12
                        }
                    }
                    Row {
                        spacing: 8
                        Text {
                            color: "#dd000000"
                            font.family: "Arial"
                            text: "Humidity:"
                            font.pointSize: 12
                        }
                        Text {
                            id: humidity
                            color: "#dd000000"
                            font.family: "Arial"
                            text: itemRectangle.humadityToText(model.humidity)
                            font.pointSize: 12
                        }
                    }
                }
            }

            Text {
                id: aditionalInfo
                color: "#dd000000"
                font.family: "Arial"
                text: model.description
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
