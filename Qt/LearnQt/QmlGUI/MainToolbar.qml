import QtQuick 2.14

Rectangle{

    property color iconColor: "purple"
    property alias title: label.text

    id: grayRect
    height: 50; width: parent.width
    color: "gray"

    Rectangle{
        id: purpleRectangle
        width: 50; height: parent.height
        color: iconColor
        radius: 50
        x:7
    }

    Text {
        id: label
        anchors.left: purpleRectangle.right
        anchors.right: parent.right
        text: qsTr("Dominate the Mobile UI")
        font.pointSize: 30
    }
}
