import QtQuick 2.14
import QtQuick.Window 2.14

Window {
    id: root
    width: 360
    height: 360
    visible: true

    // TODO: Replace with an image
    Rectangle {
        id: arrow
        color: "lightblue"
        width: root.width / 8
        height: root.height / 2
        x: root.width / 2 - width / 2
        y: root.height / 2

        rotation: 0
        smooth: true
        transformOrigin: Item.Top

        MouseArea {
            id: arrow_area
            anchors.fill: parent

            // https://stackoverflow.com/questions/2676719/calculating-the-angle-between-a-line-and-the-x-axis
            property real center_x: parent.width / 2
            property real center_y: 0

            property real delta_x: mouseX - center_x
            property real delta_y: mouseY - center_y

            property real theta_radians: Math.atan2(delta_x, delta_y)
            property real degrees: parseInt(theta_radians * 180 / Math.PI)

            onPositionChanged: {
                parent.rotation = (parent.rotation - degrees) % 360
            }
        }
    }
}
