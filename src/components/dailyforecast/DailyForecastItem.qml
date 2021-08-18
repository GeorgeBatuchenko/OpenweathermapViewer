import QtQuick 2.4
import QtGraphicalEffects 1.12

import OpenWeatherMapViewer 1.0

Item {
    width: 450
    height: 325

    property DailyForecastModel forecastModel

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
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 4

        Column {
            id: column
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            anchors.top: parent.top
            anchors.topMargin: 16

            Text {
                id: itemHeader
                height: 24
                color: "#dd000000"
                text: qsTr("Daily forecast")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                font.bold: true
                font.pointSize: 14
            }

            Text {
                id: textLoading
                color: "#dd000000"
                text: qsTr("Loading...")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: itemHeader.bottom
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                font.pointSize: 12
            }

            ListView {
                id: listView
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: itemHeader.bottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16

                model: forecastModel

                delegate: Item {
                    id: itemDelegate
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 50

                    required property string date
                    required property int temperature
                    required property string iconUrl
                    required property string description

                    Row {
                        id: row1
                        anchors.left: itemDelegate.left
                        anchors.right: itemDelegate.right
                        anchors.rightMargin: 0
                        anchors.leftMargin: 0

                        Text {
                            anchors.verticalCenter: icon.verticalCenter
                            width: 100
                            text: date
                        }
                        Image {
                            width: 50
                            height: 50
                            id: icon
                            source: iconUrl
                        }
                        Text {
                            id: textTemp
                            width: 50
                            anchors.verticalCenter: icon.verticalCenter
                            text: temperatureToText(temperature)
                        }
                        Item {
                            width: 186
                            height: 50
                            Text {
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                color: "#6e5e5e"
                                text: description
                            }
                        }
                        spacing: 8
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
