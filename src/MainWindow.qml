import QtQuick 2.12
import QtQuick.Controls 2.5

import OpenWeatherMapViewer 1.0

import "pages/home"
import "pages/cities"
import "pages/error"

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("OpenWheatherMap Viewer")

    function changeCity( cityId ) {
        AppSettings.setValue("city_id", cityId)
        if (stackView.depth > 1) {
            stackView.pop()
        } else {
            stackView.push(homePage)
        }
    }

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Component {
        id: homePage
        HomePage {

        }
    }

    Component {
        id: citiesPage
        CitiesPage {
            anchors.fill: parent
            onCityChanged: changeCity(selectedCityId)
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.33
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Choose city")
                width: parent.width
                onClicked: {
                    stackView.push(citiesPage)
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: homePage
        anchors.fill: parent
    }
}
