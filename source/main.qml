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
                Layout.fillWidth: true
            }
        }

        Rectangle {
            id: configurationRoot
            color: "lightgray"

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true

            GridLayout {
                id: grid
                anchors.fill: parent
                columns: 1

                ColumnLayout
                {
                    Layout.alignment: Qt.AlignTop

                    Button {
                        id: searchButton
                        Layout.fillWidth: true
                        text: "Search devices"
                    }

                    Row {
                        Layout.fillWidth: true
                        height: 150
                        ScrollView {
                            anchors.fill: parent
                            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                            clip: true

                            ListView {
                                model: 20
                                delegate: ItemDelegate {
                                    text: "Item " + index
                                }
                            }
                        }
                    }

                    Button {
                        id: test
                        Layout.fillWidth: true
                        text: "Search devices"
                    }
                }
            }
        }

    }
    onWidthChanged: console.log("Width " + width + " " + dial.width)
    onHeightChanged: console.log("Height " + height + " " + dial.height)
}

