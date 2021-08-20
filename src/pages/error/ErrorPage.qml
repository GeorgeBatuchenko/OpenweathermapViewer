import QtQuick 2.11
import QtQuick.Controls 2.4

Page {
    title: qsTr("Error")

    property var sourcePage
    signal retry()

    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right;
        spacing: 4

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Somthing went wrong. Try again.")
        }

        Button {
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Try again");
            onClicked: retry()
        }
    }

}
