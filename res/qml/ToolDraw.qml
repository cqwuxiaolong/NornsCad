import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Window 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

ToolBar {
    id:toolbar
    y:parent.x+height
    RowLayout {
        anchors.fill: parent
        ToolButton {
            id:drag
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
                drag.target: toolbar
                hoverEnabled: true
                onPositionChanged: {
                    if(toolbar.x<0)toolbar.x=0
                    if(toolbar.y<0)toolbar.y=0
                    if(toolbar.x>toolbar.parent.width-parent.width)toolbar.x=toolbar.parent.width-parent.width
                    if(toolbar.y>toolbar.parent.height-parent.height)toolbar.y=toolbar.parent.height-parent.height
                }
            }
        }

        ToolButton {
            id:drawNothing
            text: qsTr("DrawNothing")
            autoExclusive: true
            checkable: true
            hoverEnabled:true
            display: AbstractButton.IconOnly
            icon{source:"qrc:/nothing"}

            ToolTip{
                text: qsTr("Nothing")
                visible: parent.hovered
                y:parent.height
            }
            onCheckedChanged: {
                if(checked)
                {
                    console.log("checked")
                }
                else{
                    console.log("nochecked")
                }
            }
        }


        ToolButton {
            id:drawLine
            text: qsTr("DrawLine")
            autoExclusive: true
            checkable: true
            hoverEnabled:true
            display: AbstractButton.IconOnly
            icon{source:"qrc:/line"}

            ToolTip{
                text: qsTr("DrawLine")
                visible: parent.hovered
                y:parent.height
            }
            onCheckedChanged: {        
                viewer.drawLine(checked)
            }
        }

        ToolButton {
            id:drawPolyLine
            text: qsTr("DrawPolyLine")
            autoExclusive: true
            checkable: true
            icon{source:"qrc:/polyline.png"}
            hoverEnabled:true
            display: AbstractButton.IconOnly
            onCheckedChanged: {
                viewer.drawPolyLine(checked)
            }
            ToolTip{
                text: qsTr("DrawPolyLine")
                visible: parent.hovered
                y:parent.height
            }
        }
        ToolButton {
            id:drawArc
            text: qsTr("DrawArc")
            autoExclusive: true
            checkable: true
            icon{source:"qrc:/arc.png"}
            display: AbstractButton.IconOnly
            onCheckedChanged: {
                viewer.drawArc(checked)
            }
            ToolTip{
                text: qsTr("DrawArc")
                visible: parent.hovered
                y:parent.height
            }
        }
        ToolButton {
            id:drawCircle
            text: qsTr("DrawCircle")
            autoExclusive: true
            checkable: true
            icon{source:"qrc:/circle.png"}
            display: AbstractButton.IconOnly
            onCheckedChanged: {
                viewer.drawCircle(checked)
            }
            ToolTip{
                text: qsTr("DrawCircle")
                visible: parent.hovered
                y:parent.height
            }
        }

        ToolButton {
            id:drawRectangle
            text: qsTr("DrawRectangle")
            autoExclusive: true
            icon{source:"qrc:/rectangle"}
            display: AbstractButton.IconOnly
            checkable: true
            onCheckedChanged: {
                viewer.drawRectangle(checked)
            }
            ToolTip{
                text: qsTr("DrawRectangle")
                visible: parent.hovered
                y:parent.height
            }
        }
    }
}


