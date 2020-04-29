import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.3

Window
{
  visible: true
  width: 640; height: 480
  // A nice red rectangle

  MainToolbar{
      id: mainToolbar
      width: parent.width
      anchors.left: parent.left
      iconColor:"yellow"; title:"Dominate Qt Quick"

 }

 RowLayout{
    anchors.top: mainToolbar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

    Rectangle{width: 200; height: 200; color: "red"}
    Rectangle{width: 200; height: 200; color: "green"}
    Rectangle{width: 50; height: 50; color: "blue"}
 }


}
