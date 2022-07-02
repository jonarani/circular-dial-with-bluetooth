import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import BtDevice 1.0
import BtHandlerData 1.0

ListView {
    property alias devicesListView: devicesListView
    id: devicesListView
    visible: true
    spacing: 2
    clip: true
    focus: true

    model: BtDeviceModel {
        id: btDeviceModel
        list: _deviceList
    }


    highlight: Rectangle {
        z: 100
        color: "transparent"

        border {
            color: "black"
            width: 3
        }

        opacity: 1
    }

    delegate: Rectangle {
        id: rect
        readonly property ListView __lv: ListView.view
        color: "lightblue"
        implicitHeight: txt1.implicitHeight
        width: __lv.width

        RowLayout {
            anchors.fill: parent
            Text {
                id: txt1
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.leftMargin: 10

                text: "%1".arg(model.name)
                font.pixelSize: 25
            }
            Text {
                id: txt2
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.rightMargin: 10

                text: "%1".arg(model.address)
                font.pixelSize: 25
            }
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
