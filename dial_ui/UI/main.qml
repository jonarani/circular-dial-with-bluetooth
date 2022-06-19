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

        Dial360 {
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
            spacing: 2
            clip: true
            focus: true

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            model: BtDeviceModel {
                list: _deviceList
            }


            highlight: Rectangle {
                z: 100
                width: parent.width
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

                anchors {
                    left: parent.left
                    right: parent.right
                }

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
            visible: _btHandler.isSearchFinished

            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true

            text: "Connect"

            onClicked: {
                _btHandler.connectToDevice(_deviceList.getItemAddressAt(devicesListView.currentIndex));
            }
        }

    }

    //onWidthChanged: console.log("Width " + width + " " + dial.width)
    //onHeightChanged: console.log("Height " + height + " " + dial.height)
}

