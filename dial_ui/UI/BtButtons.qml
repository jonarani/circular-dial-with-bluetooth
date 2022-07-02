import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import BtDevice 1.0
import BtHandlerData 1.0

RowLayout {
    id: buttonRow

    property alias buttonRow: buttonRow

    implicitHeight: searchButton.height


    Button {
        id: searchButton

        Layout.fillWidth: true

        text: "Search devices"

        enabled: _btHandler.state === BtStates.IDLE ||
                 _btHandler.state === BtStates.IDLE_SEARCH_FINISHED ||
                 _btHandler.state === BtStates.DISCONNECTED ||
                 _btHandler.state === BtStates.ERROR_OCCURED

        onClicked: {
            _deviceList.removeItems();
            _btHandler.searchBtDevices();
        }
    }

    Button {
        id: connectButton
        Layout.fillWidth: true
        visible: _btHandler.state !== BtStates.CONNECTED

        text: "Connect"

        enabled: _btHandler.state === BtStates.IDLE_SEARCH_FINISHED

        onClicked: {
            _btHandler.connectToDevice(_deviceList.getItemAddressAt(btDevices.devicesListView.currentIndex));
        }
    }

    Button {
        id: disconnectButton
        Layout.fillWidth: true
        visible: _btHandler.state === BtStates.CONNECTED

        text: "Disconnect"

        enabled: _btHandler.state === BtStates.CONNECTED

        onClicked: {
            _btHandler.disconnect();
        }
    }

    Button {
        id: servoControlButton
        Layout.fillWidth: true

        text: "Go To Servo Control"


        onClicked: {
            btDevices.visible = false
            btButtons.visible = false
            servoControl.visible = true
        }
    }
}
