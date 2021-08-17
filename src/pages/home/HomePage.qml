import QtQuick 2.12
import QtQuick.Controls 2.5

import OpenWeatherMapViewer 1.0

import "../../components/currentweather"
import "../../components/hourlyforecast"
import "../../components/dailyforecast"

Page {
    title: qsTr("OpenWheatherMap Viewer")

    signal errorOccured();

    function updateModels () {
        currentWeatherModel.update()
    }

    CurrentWeatherModel {
        id: currentWeatherModel
        onHasErrorChanged: if (hasError) errorOccured()
    }

    Flow {
        id: flow1
        anchors.fill: parent

        Column {
            CurrentWeatherItem {
                id: curentWeatherItem
                model: currentWeatherModel
            }
            HourlyForecastItem {
                id: hourlyForecastItem
            }
        }

        DailyForecastItem {
            id: dailyForecastItem
        }

    }

    MouseArea {
        anchors.fill: parent
        onDoubleClicked: { updateModels() }
    }

    Component.onCompleted: updateModels()
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
