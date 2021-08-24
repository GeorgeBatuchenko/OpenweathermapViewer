import QtQuick 2.11
import QtQuick.Controls 2.4

import OpenWeatherMapViewer 1.0

import "../../components/currentweather"
import "../../components/hourlyforecast"
import "../../components/dailyforecast"

Page {
    title: qsTr("OpenWheatherMap Viewer")

    signal errorOccured();

    function updateModels () {
        currentWeatherModel.update()
        dailyForecastModel.update()
        hourlyForecastModel.update()
    }

    CurrentWeatherModel {
        id: currentWeatherModel
        onHasErrorChanged: if (hasError) errorOccured()
    }

    DailyForecastModel {
        id: dailyForecastModel
        onStateChanged: {
            if (hasError()) errorOccured()
            dailyForecastItem.setLoadingState(isQuery());
        }
    }

    HourlyForecastModel {
        id: hourlyForecastModel
        onStateChanged: {
            if (hasError()) errorOccured()
            hourlyForecastItem.setLoadingState(isQuery());
        }
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
                forecastModel: hourlyForecastModel
            }
        }

        DailyForecastItem {
            id: dailyForecastItem
            forecastModel: dailyForecastModel
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
