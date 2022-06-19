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
                _btHandler.sendMessage(rotation)
                console.log("Rotation Changed " + rotation)
            }
        }

        Button {
            id: searchButton
            Layout.fillWidth: true
            text: "Search devices"
            enabled: _btHandler.state === BtStates.IDLE ||
                     _btHandler.state === BtStates.IDLE_SEARCH_FINISHED ||
                     _btHandler.state === BtStates.DISCONNECTED ||
                     _btHandler.state === BtStates.ERROR_OCCURED



            onClicked: {
//                fillerItem.visible = false
//                devicesListView.visible = true;
                _deviceList.removeItems();
                _btHandler.searchBtDevices();
            }
        }

//        Item {
//            id: fillerItem
//            visible: true
//            Layout.fillHeight: true
//        }


        // TODO: Add StackView
        // 1st screen - listview
        // 2nd screen - servo motor params (actual degrees)

        ListView {
            id: devicesListView
            visible: true
            spacing: 2
            clip: true
            focus: true

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            model: BtDeviceModel {
                id: btDeviceModel
                list: _deviceList
            }


            highlight: Rectangle {
                z: 100
                color: "lightgreen"

                border {
                    color: "black"
                    width: 2
                }

                opacity: 0.7
            }

            delegate: Rectangle {
                readonly property ListView __lv: ListView.view
                color: "lightblue"
                implicitHeight: txt.implicitHeight
                width: __lv.width

                Text {
                    id: txt
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 7
                    }

                    text: "%1\t\t%2".arg(model.name).arg(model.address)
                    font.pixelSize: 25
                }

                MouseArea {
                    anchors {
                        fill: parent
                    }

                    onClicked: {
                        __lv.currentIndex = model.index
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                active: true
            }
        }


        Button {
            id: connectButton
            visible: _btHandler.state !== BtStates.CONNECTED

            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true

            text: "Connect"

            enabled: _btHandler.state === BtStates.IDLE_SEARCH_FINISHED

            onClicked: {
                _btHandler.connectToDevice(_deviceList.getItemAddressAt(devicesListView.currentIndex));
            }
        }

        Button {
            id: disconnectButton
            visible: _btHandler.state === BtStates.CONNECTED

            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true

            text: "Disconnect"

            enabled: _btHandler.state === BtStates.CONNECTED

            onClicked: {
                _btHandler.connectToDevice(_deviceList.getItemAddressAt(devicesListView.currentIndex));
            }
        }

    }

    //onWidthChanged: console.log("Width " + width + " " + dial.width)
    //onHeightChanged: console.log("Height " + height + " " + dial.height)
}

