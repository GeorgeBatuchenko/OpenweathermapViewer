import QtQuick 2.12
import QtQuick.Controls 2.5
import "../../components/currentweather"
import "../../components/hourlyforecast"
import "../../components/dailyforecast"

Page {
    id: homePage

    title: qsTr("OpenWheatherMap Viewer")

    Flow {
        id: flow1
        anchors.fill: parent

        Column {
            CurrentWeatherItem {
                id: curentWeatherItem
            }
            HourlyForecastItem {
                id: hourlyForecastItem
            }
        }

        DailyForecastItem {
            id: dailyForecastItem
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
