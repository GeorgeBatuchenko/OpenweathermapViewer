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
            id: column
            anchors.fill: parent
            anchors.rightMargin: 16
            anchors.leftMargin: 4
            anchors.bottomMargin: 16
            anchors.topMargin: 16
            spacing: 8

            Text {
                id: textHeader
                color: "#dd000000"
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

            ListView {
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

                model: ListModel {
                    ListElement {
                        dayPeriod: "4:00am"
                        iconFilePath: "qrc:/images/10d.png"
                        temperature: "+8C"
                    }
                    ListElement {
                        dayPeriod: "12:00am"
                        iconFilePath: "qrc:/images/10d.png"
                        temperature: "+15C"
                    }
                    ListElement {
                        dayPeriod: "8:00pm"
                        iconFilePath: "qrc:/images/10d.png"
                        temperature: "+13C"
                    }
                    ListElement {
                        dayPeriod: "8:00pm"
                        iconFilePath: "qrc:/images/10d.png"
                        temperature: "+13C"
                    }
                }

                delegate: Item {
                    width: 70
                    height: 70

                    //property string dayPeriod: qsTr("Unknown")
                    //property string iconFilePath: ""
                    //property string temperature: qsTr("Unknown")
                    Column {
                        Row {
                            anchors.horizontalCenter: weather.horizontalCenter
                            Text {
                                width: 12
                            }
                            Text {
                                text: dayPeriod
                            }
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
