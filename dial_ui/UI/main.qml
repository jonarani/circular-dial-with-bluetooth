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
            Layout.fillHeight: true
            Layout.fillWidth: true

            visible: true
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

