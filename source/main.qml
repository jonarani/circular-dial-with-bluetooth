import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Window {
    id: root
    width: 1000
    height: 1000
    visible: true

    minimumWidth: dial.main_back_width

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 3

        spacing: 5

        Dial {
            id: dial
            width: main_back_width
            height: main_back_height
            clip: true

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        }

        Rectangle {
            color: "lightgray"
            Layout.fillWidth: true
            Layout.fillHeight: true

            GridLayout {
                id: grid
                Layout.fillWidth: true
                Layout.fillHeight: true
                anchors.fill: parent
                columns: 2

                ColumnLayout
                {
                    Layout.alignment: Qt.AlignTop
                    Button {
                        Layout.fillWidth: true
                        id: searchButton
                        text: "Search devices"
                    }
                }

//                TextField {
//                    id: text
//                    Layout.alignment: Qt.AlignTop
//                    Layout.fillWidth: true
//                    //Layout.fillHeight: true
//                    anchors.top: dial.bottom
//                }
            }
        }
    }
    onWidthChanged: console.log("Width " + width + " " + dial.width)
    onHeightChanged: console.log("Height " + height + " " + dial.height)
}

