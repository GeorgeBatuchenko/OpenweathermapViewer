import QtQuick 2.4
import QtGraphicalEffects 1.0

import OpenWeatherMapViewer 1.0

Item {
    width: 465
    height: 135

    property HourlyForecastModel forecastModel;

    function setLoadingState( en ) {
        if (en === true) {
            textLoading.visible=true;
            listView.visible=false;
        } else {
             textLoading.visible=false;
             listView.visible=true;
        }
    }

    function temperatureToText(temp) {
        if(temp > 0) {
            return qsTr("+") + Number(temp).toLocaleString(Qt.locale(),"f", 0) + qsTr("℃")
        } if(temp < 0) {
            return qsTr("-") + Number(temp).toLocaleString(Qt.locale(),"f", 0) + qsTr("℃")
        } else {
            return Number(temp).toLocaleString(Qt.locale(),"f", 0) + qsTr("℃")
        }
    }

    Rectangle {
        id: itemRectangle
        color: "#ffffff"
        border.width: 0
        anchors.fill: parent
        anchors.rightMargin: 4
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4

        Rectangle {
            id: column
            anchors.fill: parent
            anchors.rightMargin: 16
            anchors.leftMargin: 4
            anchors.bottomMargin: 16
            anchors.topMargin: 16

            Text {
                id: textHeader
                color: "#dd000000"
                font.family: "Arial"
                text: qsTr("Hourly forecast")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 16
                anchors.leftMargin: 12
                anchors.topMargin: 0
                font.bold: true
                font.pointSize: 14
                height: 34
            }

            Text {
                id: textLoading
                font.family: "Arial"
                text: qsTr("Loading...")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: textHeader.bottom
                anchors.rightMargin: 16
                anchors.leftMargin: 12
                anchors.topMargin: -12
                font.pointSize: 12
                height: 18
            }

            ListView {
                id: listView
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: textHeader.bottom
                anchors.bottom: parent.bottom
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.bottomMargin: 0
                anchors.topMargin: 0

                orientation: ListView.Horizontal
                spacing: 42

                model: forecastModel

                delegate: Item {
                    width: 70
                    height: 70

                    Column {
                        Item {
                            width: 70
                            height: 18
                            anchors.horizontalCenter: weather.horizontalCenter
                            Row {
                                Text {
                                    width: 12
                                }
                                Text {
                                    width: 58
                                    height: 18
                                    font.family: "Arial"
                                    font.pointSize: 12
                                    text: time
                                }
                            }
                        }
                        Item {
                            id: weather
                            width: 70
                            height: 52
                            Row {
                                Image {
                                    width: 50
                                    height: 50
                                    id: icon
                                    source: iconUrl
                                }
                                Text {
                                    anchors.verticalCenter: icon.verticalCenter
                                    font.family: "Arial"
                                    font.pointSize: 12
                                    text: temperatureToText(temperature)
                                }
                            }
                        }
                    }
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
