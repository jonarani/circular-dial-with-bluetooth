import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import BtHandlerData 1.0

ColumnLayout {
    spacing: 10

    Row {
        Layout.fillWidth: true
        spacing: 3
        Rectangle {
            implicitHeight: goBackButton.implicitHeight
            implicitWidth: parent.width / 2
            color: "lightgray"
            Text {
                id: rotationText
                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                }

                text: "Rotation: <b>%1</b>".arg(dial.rotation < 0 ? 360.0 + dial.rotation : dial.rotation)
                font.family: "Times New Roman"
                font.pixelSize: 14
            }
        }

        Button {
            id: goBackButton
            text: "Go Back"
            width: parent.width / 2
            onClicked: {
                servoControl.visible = false
                btButtons.visible = true
                btDevices.visible = true
            }

            font.family: "Times New Roman"
            font.pixelSize: 14
        }
    }

    Row {
        spacing: 2
        Layout.fillWidth: true

        ComboBox {
            id: stepSizeComboBox
            width: parent.width / 2
            displayText: "Step Size: " + currentText
            model: [0.45, 0.90, 1.80]

            onCurrentValueChanged: {
                scannableAreaSpinBox.value = 0
            }

            currentIndex: 2

            font.family: "Times New Roman"
            font.pixelSize: 14
        }

        ComboBox {
            id: stepSpeedComboBox
            width: parent.width / 2
            displayText: "Step Speed: " + currentText + "ms"
            model: [1, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]

            font.family: "Times New Roman"
            font.pixelSize: 14
        }
    }

    Row {
        Layout.fillWidth: true
        spacing: 3
        CheckBox {
            id: autoControlCheckBox
            width: parent.width / 2
            text: qsTr("Automatic Control")

            font.family: "Times New Roman"
            font.pixelSize: 14
        }

        Rectangle {
            width: parent.width / 2
            height: parent.height
            Text {
                id: areaScanText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                }
                text: "Area To Scan:"

                font.family: "Times New Roman"
                font.pixelSize: 14
            }

            SpinBox {
                id: scannableAreaSpinBox
                //width: parent.width / 2
                anchors {
                    right: parent.right
                    rightMargin: 5
                    left: areaScanText.right
                    leftMargin: 15
                }

                font.family: "Times New Roman"
                font.pixelSize: 14

                from: 0
                value: 0
                to: 360 * 100
                stepSize: stepSizeComboBox.currentValue * 100

                editable: true
                enabled: autoControlCheckBox.checked

                property int decimals: 2
                property real realValue: value / 100.

                validator: DoubleValidator {
                    bottom: Math.min(scannableAreaSpinBox.from, scannableAreaSpinBox.to)
                    top:  Math.max(scannableAreaSpinBox.from, scannableAreaSpinBox.to)
                }

                textFromValue: function(value, locale) {
                    return Number(value / 100).toLocaleString(locale, 'f', scannableAreaSpinBox.decimals)
                }

                valueFromText: function(text, locale) {
                    return Number.fromLocaleString(locale, text) * 100
                }

                onValueChanged: {
                    var mod = value % stepSize
                    if (mod !== 0) {
                        if (mod < stepSize / 2) {
                            value = value - mod
                        }
                        else {
                            value = value + (stepSize - mod)
                        }
                    }
                }
            }
        }
    }

    Button {
        Layout.fillWidth: true
        font.family: "Times New Roman"
        font.pixelSize: 14

        //enabled: _btHandler.state === BtStates.CONNECTED

        text: "Send Config To MCU"

        onClicked: {
            _btHandler.sendConfig(stepSizeComboBox.currentValue, stepSpeedComboBox.currentValue,
                                  autoControlCheckBox.checked, scannableAreaSpinBox.realValue)

            dial.rotation = 0.0
            dial.arrow_area.snap = stepSizeComboBox.currentValue

            if (autoControlCheckBox.checked)
                dial.enabled = false
            else
                dial.enabled = true
        }
    }


    Item {
        Layout.fillHeight: true
    }
}

