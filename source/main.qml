import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import BtDevice 1.0

Window {
    id: root
    width: 530
    height: 800
    visible: true

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 5

        Dial {
            id: dial
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
        }

        Button {
            id: searchButton
            Layout.fillWidth: true
            text: "Search devices"

            onClicked: {
                fillerItem.visible = false
                devicesListView.visible = true;
                connectButton.visible = true;
                _btHandler.searchBtDevices();
            }
        }

        Item {
            id: fillerItem
            visible: true
            Layout.fillHeight: true
        }


        ListView {
            id: devicesListView
            visible: false
            spacing: 5
            clip: true

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            model: BtDeviceModel {
                list: _deviceList
            }

            delegate: Rectangle {
                width: 40
                height: 40
                color: "lightblue"

                anchors {
                    left: parent.left
                    right: parent.right
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "%1\t\t%2".arg(model.name).arg(model.address)
                    font.pixelSize: 25
                }
            }

            ScrollBar.vertical: ScrollBar {
                active: true
            }
        }


        Button {
            id: connectButton
            visible: false
            Layout.alignment: Qt.AlignBottom

            Layout.fillWidth: true
            text: "Connect"

            onClicked: {
                //btHandler.connectToDevice();
            }
        }

    }

    //onWidthChanged: console.log("Width " + width + " " + dial.width)
    //onHeightChanged: console.log("Height " + height + " " + dial.height)
}

