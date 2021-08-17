import QtQuick 2.12
import QtQuick.Controls 2.5

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
            text: "Somthing went wrong. Try again."
        }

        Button {
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Try again");
            onClicked: retry()
        }
    }
}
