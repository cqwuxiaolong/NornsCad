import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Window 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

ToolBar {
    id:boolToolsBar
    y:parent.x+height
    RowLayout {
        anchors.fill: parent
        ToolButton {
            id:boolToolsDrag
            text: qsTr("Drag")
            checkable: false
            hoverEnabled:true
            display: AbstractButton.IconOnly
            icon {source:"qrc:/drag.png"}
            ToolTip{
                text: qsTr("Drag")
                visible: parent.hovered
                y:parent.height
            }
            MouseArea{
                anchors.fill: parent
                drag.target: boolToolsBar
                hoverEnabled: true
                onPositionChanged: {
                    if(boolToolsBar.x<0)boolToolsBar.x=0
                    if(boolToolsBar.y<0)boolToolsBar.y=0
                    if(boolToolsBar.x>boolToolsBar.parent.width-parent.width)boolToolsBar.x=boolToolsBar.parent.width-parent.width
                    if(boolToolsBar.y>boolToolsBar.parent.height-parent.height)boolToolsBar.y=boolToolsBar.parent.height-parent.height
                }
            }
        }

        ToolButton {
            id:boolCut
            text: qsTr("Cut")
            autoExclusive: true
            checkable: true
            hoverEnabled:true
            display: AbstractButton.IconOnly
            icon{source:"qrc:/cut.png"}

            ToolTip{
                text: qsTr("Cut")
                visible: parent.hovered
                y:parent.height
            }
            
        }


        ToolButton {
            id:boolCommon
            text: qsTr("Common")
            autoExclusive: true
            checkable: true
            hoverEnabled:true
            display: AbstractButton.IconOnly
            icon{source:"qrc:/common.png"}

            ToolTip{
                text: qsTr("Common")
                visible: parent.hovered
                y:parent.height
            }
            onCheckedChanged: {        
                //viewer.drawLine(checked)
            }
        }

        ToolButton {
            id:boolFuse
            text: qsTr("Fuse")
            autoExclusive: true
            checkable: true
            icon{source:"qrc:/fuse.png"}
            hoverEnabled:true
            display: AbstractButton.IconOnly
            onCheckedChanged: {
                viewer.drawPolyLine(checked)
            }
            ToolTip{
                text: qsTr("Fuse")
                visible: parent.hovered
                y:parent.height
            }
        }
    }
}


