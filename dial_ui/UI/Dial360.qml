import QtQuick 2.0

Item {
    id: dial
    signal arrowRotationChanged(real rotation);

    property real _scale: 1.0
    property int main_back_width: main_back.width * _scale
    property int main_back_height: main_back.height * _scale

    // The purpose of snap is that arrow could be moved in increments of 1.8 degrees.
    // Servo motor one full step is 1.8 degrees
    // The motor has half step and quarter step possibilities
    property alias arrow_area: arrow_area

    property alias rotation: arrow.rotation
    property real prevRotation: 0.0;

    property string imagesPath: "qrc:/UI/images/"

    width: main_back_width
    height: main_back_height - 90
    clip: true

    Image {
        id: background
        scale: 1
        source: imagesPath + "background.png"
        anchors {
            fill: parent
        }
    }

    Image {
        id: main_back
        source: imagesPath + "mainBack.png"
        scale: _scale
        anchors {
            horizontalCenter: background.horizontalCenter
            top: background.top
        }
    }

    Image {
        id: main_front
        source: imagesPath + "mainFront.png"
        scale: _scale
        anchors {
            centerIn: green_circle
        }
    }

    Image {
        id: green_circle
        source: imagesPath + "circular-dial.png"
        scale: _scale
        anchors {
            horizontalCenter: background.horizontalCenter
            top: background.top
        }
    }

    Image {
        id: support
        source: imagesPath + "knob.png"
        scale: _scale
        z: 1
        anchors {
            centerIn: green_circle
        }
    }

    Image {
        id: arrow
        source: imagesPath + "arrow.png"
        scale: _scale
        x: support.x + support.width / 2 - arrow.width / 2
        y: support.y + support.height / 2 - arrow.height
        z: 0

        smooth: true
        transformOrigin: Item.Bottom

        MouseArea {
            id: arrow_area
            anchors.fill: parent

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

                if (prevRotation != parent.rotation)
                {
                    prevRotation = parent.rotation;
                    dial.arrowRotationChanged(parent.rotation);
                }
            }
        }
    }
}
