import QtQuick 2.14
import QtQuick.Window 2.14

Window {
    id: root
    width: dial.main_back_width
    height: 700
    visible: true

    // TODO: Vertical top half consists of dial
    //       Vertical bottom half consists of options and configurations


    Dial {
        id: dial
        width: main_back_width
        height: main_back_height
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: rec.top
        }
    }

    Rectangle {
        id: rec
        width: 100
        height: 100
        color: "red"

        anchors {
            top: dial.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}
