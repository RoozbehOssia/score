import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
Item {

    TreeView {
        id: nodeView
        model: nodesModel
        objectName: "nodeView"
        anchors.fill: parent

        TableViewColumn {
            title: "Address"
            role: "address"
            width: 60
        }
        TableViewColumn {
            title: "Value"
            role: "value"
            width: 60
        }

        Component {
            id: treeDelegate

            ColumnLayout
            {
                height:70
                id: delegateLayout

                MouseArea {
                    id: mouseArea
                    anchors.fill: text
                    drag.target: draggable
                }

                Text {
                    id: text
                    anchors.verticalCenter: parent.verticalCenter
                    color: styleData.textColor
                    elide: styleData.elideMode
                    text: styleData.value

                    Item {
                        id: draggable
                        width: 0
                        height: 0
                        anchors.fill: parent

                        Drag.active: mouseArea.drag.active
                        Drag.hotSpot.x: 0
                        Drag.hotSpot.y: 0

                        Drag.mimeData: { "iscore/x-remote-address": nodesModel.nodeToAddressString(styleData.index) }
                        Drag.dragType: Drag.Automatic
                    }
                }

            }
        }

        itemDelegate: treeDelegate

    }

}
