import QtQuick 2.12
import QtQuick.Controls 2.5

import OpenWeatherMapViewer 1.0

Page {
    title: qsTr("Choose city")

    signal cityChanged(string selectedCityId)
    signal errorOccured()

    function updateModels(){}

    CitiesListModel {
        id: citiesModel
        onStateChanged: if (citiesModel.hasError()) errorOccured()
    }

    TextField {
        id: textFieldCity
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        placeholderText: qsTr("Enter city name")
    }

    Button {
        id: buttonFind
        text: qsTr("Find")
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: textFieldCity.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        onClicked: citiesModel.update(textFieldCity.text)
    }

    ListView {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: buttonFind.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0

        model: citiesModel

        delegate: Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#e6e9f0"
                }

                GradientStop {
                    position: 1
                    color: "#eef1f5"
                }
            }
            height: 50
            color: "#c4c5c7"

            required property int index
            required property string cityName
            required property int cityId
            required property double cityLon
            required property double cityLat

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                text: cityName
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!citiesModel.isNotFound()) {
                        citiesModel.saveItem(index)
                        cityChanged(cityId)
                    }
                }
            }
        }
    }
}
