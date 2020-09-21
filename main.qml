import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import org.orgname.BackEnd 1.0

Window {
    id: window
    width: 480
    height: 320
    visible: true

    title: qsTr("Serial Port Reader")

    minimumHeight: height
    minimumWidth: width
    maximumHeight: minimumHeight
    maximumWidth: minimumWidth

    Column {
        id: column
        spacing: 10
        anchors.margins: 10
        anchors.fill: parent

        ComboBox {
            id: comboBox
            font.pointSize: 10
            popup.font.pointSize: 10
            flat: true
            anchors.right: parent.right
            anchors.left: parent.left
            model: BackEnd.availablePortsNames
            currentIndex: BackEnd.selectedPortIndex
            onCurrentIndexChanged: BackEnd.selectedPortIndex = currentIndex
            displayText: currentIndex == -1 ? qsTr("Select port") : currentText

            Connections {
                target: comboBox.popup
                function onAboutToShow() {
                    BackEnd.updateAvailablePorts()
                }
                function onAboutToHide() {
                    BackEnd.updateAvailablePorts()
                }
            }
        }

        Flickable {
            anchors.right: parent.right
            anchors.left: parent.left
            height: 250
            flickableDirection: Flickable.AutoFlickIfNeeded

            TextArea.flickable: TextArea {
                id: textArea
                wrapMode: TextEdit.WrapAnywhere
                font.pointSize: 10
                readOnly: true
                selectByMouse: true
                text: BackEnd.readingHistory
            }

            ScrollBar.vertical: ScrollBar {}
        }
    }
}
