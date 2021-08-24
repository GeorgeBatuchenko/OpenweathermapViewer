import QtQuick 2.11
import QtQuick.Controls 2.4

import OpenWeatherMapViewer 1.0

import "pages/home"
import "pages/cities"
import "pages/error"

ApplicationWindow {
    id: window
    width: 465
    height: 930
    visible: true
    title: qsTr("OpenWheatherMap Viewer")

    function changeCity( cityId ) {
            stackView.pop()
    }

    function switchToPage( page ) {
        stackView.pop()
        stackView.push(page)
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
            if (stackView.currentItem != errorPage) {
                errorPage.sourcePage = homePage
                switchToPage(errorPage)
            }
        }
    }

    CitiesPage {
        id: citiesPage
        anchors.fill: parent
        onCityChanged: changeCity(selectedCityId)
        onErrorOccured: {
            if (stackView.currentItem != errorPage) {
                errorPage.sourcePage = citiesPage
                switchToPage(errorPage)
            }
        }
    }

     ErrorPage {
         id: errorPage
         anchors.left: parent.left
         anchors.right: parent.right;
         onRetry: {
             if (errorPage.sourcePage === homePage) {
                 stackView.pop();
             } else {
                 switchToPage(errorPage.sourcePage)
             }
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
                    stackView.pop();
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

//    Component.onCompleted: {
//        stackView.push(errorPage)
//        stackView.push(errorPage2)
//        console.log("SD ",stackView.depth)
//    }
}
