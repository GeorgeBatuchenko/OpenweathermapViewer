import QtQuick 2.4
import QtGraphicalEffects 1.12

Item {
    width: 450
    height: 325

    Rectangle {
        id: itemRectangle
        color: "#ffffff"
        border.width: 0
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.bottom:  parent.bottom
        anchors.bottomMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 4

        Column {
            id: column
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.bottom:  parent.bottom
            anchors.bottomMargin: 16
            anchors.top: parent.top
            anchors.topMargin: 16

            Text {
                id: itemHeader
                color: "#dd000000"
                text: qsTr("Daily forecast")
                font.bold: true
                font.pointSize: 14
            }

            ListView {
                id: listView
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: itemHeader.bottom
                anchors.bottom: parent.bottom

                //anchors.fill: parent
                model: ListModel {
                    ListElement {
                        day: "Tomorow"
                        iconFilePath: "qrc:/images/10d.png"
                        temperature: "+15C"
                        clouds: "Broken clouds"
                    }

                    ListElement {
                        day: "12.08.2020"
                        iconFilePath: "qrc:/images/10d.png"
                        temperature: "+16C"
                        clouds: "Broken clouds"
                    }

                    ListElement {
                        day: "13.08.2020"
                        iconFilePath: "qrc:/images/10d.png"
                        temperature: "+16C"
                        clouds: "Broken clouds"
                    }

                    ListElement {
                        day: "15.08.2020"
                        iconFilePath: "qrc:/images/10d.png"
                        temperature: "+16C"
                        clouds: "Broken clouds"
                    }
                }

                delegate: Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 50
                    //anchors.fill: parent
                    Row {
                        id: row1
                        Text {
                            anchors.verticalCenter: icon.verticalCenter
                            width: 130
                            text: day
                        }
                        Image {
                            id: icon
                            source: iconFilePath
                        }
                        Text {
                            anchors.verticalCenter: icon.verticalCenter
                            text: temperature
                        }
                        Text {
                            anchors.verticalCenter: icon.verticalCenter
                            color: "#6e5e5e"
                            text: clouds
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
