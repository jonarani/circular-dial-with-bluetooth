import QtQuick 2.14
import QtQuick.Window 2.14

Window {
    id: root
    width: main_back.width * main_back.scale
    height: main_back.height * main_back.scale
    visible: true

    // TODO: Vertical top half consists of dial
    //       Vertical bottom half consists of options and configurations
    Image {
        id: background
        source: "dial/images/background.png"
        anchors {
            fill: parent
        }

        Image {
            id: main_back
            source: "dial/images/mainBack.png"
            scale: 2
            anchors {
                centerIn: parent
            }
        }

        Image {
            id: main_front
            source: "dial/images/mainFront.png"
            scale: 2
            anchors {
                centerIn: parent
            }
        }

        Image {
            id: green_circle
            source: "dial/images/circular-dial.png"
            scale: 2
            anchors {
                centerIn: parent
            }
        }

        Image {
            id: support
            source: "dial/images/knob.png"
            scale: 2
            z: 1
            anchors {
                centerIn: parent
            }
        }

        Image {
            id: arrow
            source: "dial/images/arrow.png"
            scale: 2
            x: support.x + support.width / 2 - arrow.width / 2
            y: support.y + support.height / 2 - arrow.height
            z: 0

            smooth: true
            transformOrigin: Item.Bottom

            MouseArea {
                id: arrow_area
                anchors.fill: parent

                // The purpose of snap is that arrow could be moved in increments of 1.8 degrees.
                // Servo motor one full step is 1.8 degrees
                // The motor has half step and quarter step possibilities
                // TODO: make this configurable
                //       possible options: 0.45, 0.9, 1.8 and 3.6?
                property real snap: 1.8

                // https://stackoverflow.com/questions/2676719/calculating-the-angle-between-a-line-and-the-x-axis
                property real center_x: arrow.width / 2
                property real center_y: arrow.height

                // Negative in front since transform origin is bottom
                property real delta_x: -(mouseX - center_x)
                property real delta_y: -(mouseY - center_y)

                property real theta_radians: Math.atan2(delta_x, delta_y)
                property real degrees: (theta_radians * 180 / Math.PI)

                onPositionChanged: {
                    // Rotate in increments of <snap> degrees. Modulo 360 to keep rotation [0; 360).
                    parent.rotation = (parseInt((parent.rotation - degrees) / snap) * snap) % 360
                    console.log (parent.rotation)
                }
            }
        }
    }
}
