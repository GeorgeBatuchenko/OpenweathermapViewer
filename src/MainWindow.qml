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
        if (stackView.depth > 1) {
            stackView.pop()
        }
    }

    function switchToPage( page ) {
        if (stackView.depth > 1) {
            stackView.pop()
            stackView.push(page)
        } else {
            stackView.push(page)
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

    HomePage {
        id: homePage
        onErrorOccured: {
            errorPage.sourcePage = homePage
            switchToPage(errorPage)
        }
    }

     CitiesPage {
        id: citiesPage
        anchors.fill: parent
        onCityChanged: changeCity(selectedCityId)
        onErrorOccured: {
            errorPage.sourcePage = citiesPage
            switchToPage(errorPage)
        }
    }

     ErrorPage {
         id: errorPage
         anchors.fill: parent
         onRetry: {
             switchToPage(errorPage.sourcePage)
             errorPage.sourcePage.updateModels()
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
