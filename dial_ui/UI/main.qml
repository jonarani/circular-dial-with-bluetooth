import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import BtDevice 1.0
import BtHandlerData 1.0

Window {
    id: root
    width: 530
    height: 800
    visible: true

    Connections {
        target: _btHandler
        function onDataReceivedViaBt(data) {
            textArea.text = textArea.text + "Received data from BT: " + data
        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 5

        Dial360 {
            id: dial

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

            onArrowRotationChanged: {
                _btHandler.addToQueue(rotation)
            }
        }

        BtButtons {
            id: btButtons
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

            visible: true
        }

        BtDevicesList {
            id: btDevices
            //Layout.fillHeight: true
            height: 100
            Layout.fillWidth: true
            visible: true
        }

        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                id: textArea
                readOnly: true
                font.family: "Courier"
                font.pixelSize: 12

                text: ""
            }
        }

        ServoControl {
            id: servoControl

            Layout.fillHeight: true
            Layout.fillWidth: true
            //Layout.alignment: Qt.AlignTop

            visible: false
        }
    }
}

