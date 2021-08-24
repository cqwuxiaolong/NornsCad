// Created: 2014-08-28
//
// Copyright (c) 2014 OPEN CASCADE SAS
//
// This file is part of commercial software by OPEN CASCADE SAS.
//
// This software is furnished in accordance with the terms and conditions
// of the contract and with the inclusion of this copyright notice.
// This software or any other copy thereof may not be provided or otherwise
// be made available to any third party.
// No ownership title to the software is transferred hereby.
//
// OPEN CASCADE SAS makes no representation or warranties with respect to the
// performance of this software, and specifically disclaims any responsibility
// for any damages, special or consequential, connected with its use.

import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Window 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import QtQuick.Dialogs 1.2
import AndroidQt 1.0


ApplicationWindow {
    id: root_window
    visible: true
    width:  (Qt.platform.os == "android" || Qt.platform.os == "ios") ? Screen.width : 1024
    height: (Qt.platform.os == "android" || Qt.platform.os == "ios") ? Screen.height : 768
    function evel(cmd)
    {
        viewer.commandLine=cmd
    }
    function getResult()
    {
        return viewer.commandLine
    }
    menuBar: MenuBar {
        Menu {
            id:menuFile
            title: qsTr("&File")
            Action { text: qsTr("&New...") }
            Action { text: qsTr("&Open...") ;onTriggered: file_dialog.open()}
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("Cu&t") }
            Action { text: qsTr("&Copy") }
            Action { text: qsTr("&Paste") }
        }

        Menu{
            title: qsTr("&View")
            MenuItem{
                id:acFront
                text: qsTr("Front")
                //display: AbstractButton.IconOnly
                icon { source:"qrc:/view-front"}
                onClicked: viewer.front()
                ToolTip{
                    text: qsTr("Front")
                    visible: parent.hovered
                    y:parent.height
                    x:parent.x+48
                }
            }

            MenuItem{
                id:acBack
                text: qsTr("Back")
                //display: AbstractButton.IconOnly
                icon {source:"qrc:/view-rear"}
                onClicked: viewer.back()
                ToolTip{
                    text: qsTr("Back")
                    visible: parent.hovered
                    y:parent.height
                    x:parent.x+48
                }
            }

            MenuItem{
                id:acLeft
                text: qsTr("Left")
                //display: AbstractButton.IconOnly
                icon { source:"qrc:/view-left"}
                onClicked: viewer.left()
                ToolTip{
                    text: qsTr("Left")
                    visible: parent.hovered
                    y:parent.height
                    x:parent.x+48
                }
            }

            MenuItem{
                id:acRight
                text: qsTr("Right")
                //display: AbstractButton.IconOnly
                icon { source:"qrc:/view-right"}
                onClicked: viewer.right()
                ToolTip{
                    text: qsTr("Right")
                    visible: parent.hovered
                    y:parent.height
                    x:parent.x+48
                }
            }

            MenuItem{
                id:acTop
                text: qsTr("Top")
                //display: AbstractButton.IconOnly
                icon { source:"qrc:/view-top"}
                onClicked: viewer.top()
                ToolTip{
                    text: qsTr("Top")
                    visible: parent.hovered
                    y:parent.height
                    x:parent.x+48
                }
            }

            MenuItem{
                id:acBottom
                text: qsTr("Bottom")
                //display: AbstractButton.IconOnly
                icon { source:"qrc:/view-bottom"}
                onClicked: viewer.bottom()
                ToolTip{
                    text: qsTr("Bottom")
                    visible: parent.hovered
                    y:parent.height
                    x:parent.x+48
                }
            }

            MenuItem{
                id:acAxo
                text: qsTr("Axo")
                //display: AbstractButton.IconOnly
                icon { source:"qrc:/view-axonometric"}
                onClicked: viewer.axo()
                ToolTip{
                    text: qsTr("Axo")
                    visible: parent.hovered
                    y:parent.height
                    x:parent.x+48
                }
            }

            MenuSeparator { }
            MenuItem{
                text: qsTr("FitAll")
                //display: AbstractButton.IconOnly
                icon { source:"qrc:/view-fullscreen"}
                onClicked: viewer.fitAll()
                ToolTip{
                    text: qsTr("Front")
                    visible: parent.hovered
                    y:parent.height
                    x:parent.x+48
                }
            }

        }
        Menu{
            title: "Selection mode"
            Action{text: qsTr("Vertices");onTriggered:viewer.selectVertices()}
            Action{text: qsTr("Edges");onTriggered:viewer.selectEdges()}
            Action{text: qsTr("Faces");onTriggered:viewer.selectFaces()}
            Action{text: qsTr("Wires");onTriggered:viewer.selectWires()}
            Action{text: qsTr("Shapes");onTriggered:viewer.selectShapes()}
            Action{text: qsTr("Shells");onTriggered:viewer.selectShelles()}
            Action{text: qsTr("Solids");onTriggered:viewer.selectSolid()}
            Action{text: qsTr("Compsolids");onTriggered:viewer.selectCompsolid()}
            Action{text: qsTr("Compounds");onTriggered:viewer.selectCompound()}
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
        }
    }
    Item {
        id: root_item
        anchors.fill: parent
        AndroidQt {
            id: viewer
        }

        Rectangle{
            id:rubberBand
            visible:false
            opacity:0.1
            property int sx//start mouseX
            property int sy//start mouseY
            property bool allow
            function initRubberBand(mx,my)//mouseX mouseY
            {
                height=0
                width=0
                sx=mx
                sy=my
                x=mx
                y=my
                visible=true
                allow=true
            }
            function upDateRubberBand(mx,my)
            {
                if(!allow)return;
                height=Math.abs(my-y)
                if(my>sy)
                {
                    height=my-y
                }
                else
                {
                    height=sy-my
                    y=my
                }
                if(mx>sx)
                {
                    width=mx-x
                }
                else
                {
                    width=sx-mx
                    x=mx
                }
            }
        }

        Keys.onEnterPressed: {
             viewer.confirmPolyLine()
        }
        Keys.onReturnPressed:{
            viewer.confirmPolyLine()
        }


        MouseArea {
            id:viewerarea
            anchors.fill: parent
            hoverEnabled:true
            acceptedButtons: Qt.RightButton|Qt.LeftButton |Qt.MiddleButton
            onPressed:
            {
                viewer.initTouch(mouseX, mouseY)

                if(mouse.button==Qt.MiddleButton)
                {
                    viewer.rotation(true)
                }
                if(mouse.button==Qt.LeftButton)
                {
                    viewer.leftButtonPressed()
                    rubberBand.initRubberBand(mouseX,mouseY)
                }
                if(mouse.button==Qt.RightButton)
                {
                    viewer.rightButtonPressed()
                }
                if(mouse.buttons==(Qt.RightButton|Qt.LeftButton))
                {
                    viewer.pan(true)
                    rubberBand.visible=false
                    rubberBand.allow=false
                }

            }
            onReleased: {
                viewer.pan(false)
                viewer.rotation(false)
                viewer.mouseButtonReleased()
                rubberBand.visible=false
            }
            onPositionChanged:{
                viewer.updateTouch (mouseX, mouseY+41)
                rubberBand.upDateRubberBand(mouseX,mouseY)
            }

            onWheel: {
                viewer.zoom(wheel.angleDelta.y/20,true)
            }

            onDoubleClicked: {
                viewer.mouseDoubleClicked()
            }
        }

        Popup{
            id:transParencyPopup
            x:toolbar.x+transParency.x
            y:toolbar.y+transParency.height
            height: transParency.height
            width:200
            background: Rectangle {
                opacity:0.2
            }
            Slider{
                id:transParencySlider
                anchors.fill:parent
                stepSize: 1;
                from: 0
                to: 10
                orientation:Qt.Horizontal
                snapMode:"SnapAlways"
                onValueChanged: {
                    viewer.transParency =value/10
                }

            }

        }

        Popup{
            id:colorPalettePopup
            x:toolbar.x+colorPalette.x-80
            y:toolbar.y+colorPalette.height
            width:245
            height:width
            background:Rectangle{
                anchors.fill: parent
                opacity:0.2
            }
            onClosed: viewer.setOcctColorComplete();
            ColorSelector {
                anchors.fill:parent
                id: colorChooser
            }
            
        }
       
       
        

        ToolBar {
            id:toolbar
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    id:drag
                    text: qsTr("Drag")
                    checkable: true
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
                    id:hlrSwitching
                    text: qsTr("HlrSwitching")
                    checkable: true
                    hoverEnabled:true
                    display: AbstractButton.IconOnly
                    icon{source:"qrc:/view_comp_off"}

                    ToolTip{
                        text: qsTr("HlrSwitching")
                        visible: parent.hovered
                        y:parent.height
                    }
                    onCheckedChanged: {
                        if(checked==false)
                        {
                            viewer.hlr(false);
                        }
                        if(checked==true)
                        {
                            viewer.hlr(true);
                        }
                    }
                }

                ToolButton {
                    id:shading
                    text: qsTr("Shading")
                    icon{source:"qrc:/tool_shading"}
                    hoverEnabled:true
                    display: AbstractButton.IconOnly
                    onClicked: viewer.shading()
                    ToolTip{
                        text: qsTr("Shading")
                        visible: parent.hovered
                        y:parent.height
                    }
                }
                ToolButton {
                    id:wireFrame
                    text: qsTr("WireFrame")
                    icon{source:"qrc:/tool_wireframe"}
                    display: AbstractButton.IconOnly
                    onClicked: viewer.wireframe()
                    ToolTip{
                        text: qsTr("WireFrame")
                        visible: parent.hovered
                        y:parent.height
                    }
                }
                ToolButton {
                    id:transParency
                    text: qsTr("TransParency")                    
                    icon{source:"qrc:/tool_transparency"}
                    display: AbstractButton.IconOnly
                    onClicked: {
                        transParencySlider.value=viewer.transParency*10
                        transParencyPopup.open()
                    }
                    ToolTip{
                        text: qsTr("TransParency")
                        visible: parent.hovered
                        y:parent.height
                    }
                }
                
                ToolButton {
                    id:colorPalette
                    text: qsTr("ColorPalette")                    
                    icon{source:"qrc:/color_palette"}
                    display: AbstractButton.IconOnly
                    // checkable: true
                    onClicked:{
                        colorPalettePopup.open()
                    }
                    // onCheckedChanged: {
                    //     //colorChooser.visible=checked
                        
                    // }
                    ToolTip{
                        text: qsTr("ColorPalette")
                        visible: parent.hovered
                        y:parent.height
                    }
                }

                ToolSeparator {}

                ToolButton {
                    text: qsTr("Ray-tracing")
                    icon{source:"qrc:/raytracing"}
                    display: AbstractButton.IconOnly
                    checkable: true
                    onCheckedChanged: {
                        if(checked)viewer.setRaytracing(true)
                        else viewer.setRaytracing(false)
                    }
                    ToolTip{
                        text: qsTr("Ray-tracing")
                        visible: parent.hovered
                        y:parent.height
                    }
                }
                ToolButton {
                    text: qsTr("Shadows")
                    icon{source:"qrc:/shadows"}
                    display: AbstractButton.IconOnly
                    checkable: true
                    onCheckedChanged: {
                        if(checked)viewer.setRaytracedShadows (true)
                        else viewer.setRaytracedShadows(false)
                    }
                    ToolTip{
                        text: qsTr("Shadows")
                        visible: parent.hovered
                        y:parent.height
                    }
                }
                ToolButton {
                    text: qsTr("Reflections")
                    icon{source:"qrc:/reflections"}
                    display: AbstractButton.IconOnly
                    checkable: true
                    onCheckedChanged: {
                        if(checked)viewer.setRaytracedReflections (true)
                        else viewer.setRaytracedReflections(false)
                    }
                    ToolTip{
                        text: qsTr("Reflections")
                        visible: parent.hovered
                        y:parent.height
                    }
                }
                ToolButton {
                    text: qsTr("Anti-aliasing")
                    icon{source:"qrc:/antialiasing"}
                    display: AbstractButton.IconOnly
                    checkable: true
                    onCheckedChanged: {
                        if(checked)viewer.setRaytracedAntialiasing (true)
                        else viewer.setRaytracedAntialiasing(false)
                    }
                    ToolTip{
                        text: qsTr("Anti-aliasing")
                        visible: parent.hovered
                        y:parent.height
                    }
                }
                ToolSeparator{}
                ToolButton {
                    text: qsTr("Undo")
                    icon{source:"qrc:/undo.png"}
                    display: AbstractButton.IconOnly
                    onClicked:viewer.undo()
                    ToolTip{
                        text: qsTr("Undo")
                        visible: parent.hovered
                        y:parent.height
                    }
                }
                ToolButton {
                    text: qsTr("Redo")
                    icon{source:"qrc:/redo.png"}
                    display: AbstractButton.IconOnly
                    onClicked:viewer.redo()
                    ToolTip{
                        text: qsTr("Redo")
                        visible: parent.hovered
                        y:parent.height
                    }
                }


                ToolSeparator{}

                ToolButton {
                    text: qsTr("ObjWireFrame")
                    checkable: true
                    onCheckedChanged: {
                        if(checked)
                        {
                            viewer.setAISDisplayMode (true)
                            text= qsTr("AISShaded")
                        }
                        else{
                            viewer.setAISDisplayMode(false)
                            text= qsTr("AISWireFrame")
                        }
                    }
                }
                ToolButton {
                    text: qsTr("UnSetAISMode")
                    onClicked:{
                        viewer.unSetAISDisplayMode()
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }
        


        CommandLine{
            id:commandline
        }

        ToolDraw{
            id:toolDraw
        }

        BoolToolsBar{
            id:boolToolsBar
            x:toolDraw.width
        }



        FileDialog {
            id: file_dialog
            title: "Please choose a file"
            selectMultiple: false
            nameFilters: [ "BRep files (*.brep)", "All files (*)" ]
            onAccepted: viewer.readShapeFromFile(file_dialog.fileUrl)
        }
        ////
    }

}
