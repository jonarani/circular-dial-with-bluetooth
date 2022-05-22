import QtQuick 2.0

Item {
    property real _scale: 1
    property int main_back_width: main_back.width * _scale
    property int main_back_height: main_back.height * _scale

    property int rotation: arrow.rotation


    Image {
        id: background
        scale: 1
        source: "dial/images/background.png"
        anchors {
            fill: parent
        }
    }

    Image {
        id: main_back
        source: "dial/images/mainBack.png"
        scale: _scale
        anchors {
            horizontalCenter: background.horizontalCenter
            top: background.top
        }
    }

    Image {
        id: main_front
        source: "dial/images/mainFront.png"
        scale: _scale
        anchors {
            centerIn: green_circle
        }
    }

    Image {
        id: green_circle
        source: "dial/images/circular-dial.png"
        scale: _scale
        anchors {
            horizontalCenter: background.horizontalCenter
            top: background.top
        }
    }

    Image {
        id: support
        source: "dial/images/knob.png"
        scale: _scale
        z: 1
        anchors {
            centerIn: green_circle
        }
    }

    Image {
        id: arrow
        source: "dial/images/arrow.png"
        scale: _scale
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
