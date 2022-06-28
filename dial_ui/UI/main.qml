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
        }

        // TODO: Add StackView
        // 1st screen - listview
        // 2nd screen - servo motor params (actual degrees)
        //              configure steps size - 0.45 0.9 and 1.8 degrees
        //              automatic mode and manual mode config
        //              automatic mode scans configured area (for example 180 degrees)
        //              goes to and back 180 degrees
        //              can set the step speed
        // OR just use visibility to change from ListView to ServoMotorData screen or whatever
        // When connected show Servo Control instead of list view
        // if disconnected then show listview

        BtDevicesList {
            id: btDevices
        }


    }
}

