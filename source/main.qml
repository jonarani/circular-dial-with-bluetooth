import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Window {
    id: root
    width: 1000
    height: 1000
    visible: true

    //minimumWidth: dial.main_back_width

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 3
        spacing: 5

        RowLayout {
            id: dialLayout
            Layout.alignment: Qt.AlignTop
            Dial {
                id: dial
                width: main_back_width
                height: main_back_height
                clip: true

                Layout.fillWidth: true
            }
        }

        Rectangle {
            Layout.alignment: Qt.AlignTop
            color: "lightgray"
            Layout.fillWidth: true
            Layout.fillHeight: true

            GridLayout {
                id: grid
                Layout.fillWidth: true
                Layout.fillHeight: true
                anchors.fill: parent
                columns: 1

                ColumnLayout
                {
                    Layout.rowSpan: 2
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true

                    Button {
                        id: searchButton
                        Layout.fillWidth: true
                        text: "Search devices"

                        onPressed: console.log ("Pressed")
                    }

                    Row {
                        Layout.fillWidth: true
                        height: 150
                        ScrollView {
                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.rowSpan: 2
                            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                            clip:true

                            ListView {
                                model: 20
                                delegate: ItemDelegate {
                                    text: "Item " + index
                                }
                            }
                        }
                    }
                }
            }
        }

    }
    onWidthChanged: console.log("Width " + width + " " + dial.width)
    onHeightChanged: console.log("Height " + height + " " + dial.height)
}

