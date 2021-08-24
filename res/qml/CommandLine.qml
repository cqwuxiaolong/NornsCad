import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.3

Rectangle{
    id: commandItem
    width:parent.width*.8
    height:50
    color:"#3b3b3b"
    radius: height
    opacity: .9
    y:root_window.height-2*height
    x:root_window.width*.1
    Material.accent: Material.Blue
    Material.primary: Material.Indigo
    ScrollView {
        id:commandLineLogView
        height: 200
        visible: false
        opacity:parent.opacity
        width: parent.width-parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.top

        background: Rectangle { color: "#77777a" }
        TextArea{
            id:commandLineLog
            opacity:parent.opacity
            cursorVisible: true;
            wrapMode: TextEdit.Wrap
            anchors.bottomMargin: 0//换行
            font.pixelSize: 25;
            focus: true;
            textFormat:TextEdit.AutoText
            selectByMouse:true;
            selectByKeyboard: true
            color: "#c8c5d3"
            readOnly: true
            cursorPosition : length-1
            objectName: "commandLineLog"
            
            states: [
                State {
                    name: "isShow"
                    PropertyChanges {
                        target: commandLineLogView
                        visible:true
                    }
                },
                State {
                    name: "isHide"
                    PropertyChanges {
                        target: commandLineLogView
                        visible:false
                    }
                }
            ]
        }

    }
    Image {
        id: commandLineEject
        width: parent.height
        height: parent.height
        anchors.right: parent.right
        source: "qrc:/eject.png"
        MouseArea{
            id:commandLineLogEjectMouse
            anchors.fill: parent
            onClicked: {
                commandLineLog.state=commandLineLog.state!="isShow"?"isShow":"isHide"
            }
        }
    }
    Image {
        id: commandLineDrag
        width: parent.height
        height: parent.height
        anchors.left: parent.left
        source: "qrc:/drag.png"

    }
    TextInput{
        id:commandLineInput
        width: parent.width-2*parent.height
        opacity: parent.opacity
        anchors.left:commandLineDrag.right
        height: parent.height
        font.pixelSize: 25
        verticalAlignment: Text.AlignVCenter
        selectByMouse: true
        Keys.onEnterPressed: {
            if(text.trim()!="")
            {
                
                commandLineLog.append("<font color=\"#FFFFFF\">"+text+"<\font>")
                root_window.evel(commandLineInput.text)
                if(root_window.getResult()!="")
                {
                    commandLineLog.append("<font color=\"#00FFFF\">"+root_window.getResult()+"<\font>")
                }
                
            }
            commandLineInput.text=""
        }
        Keys.onReturnPressed:
        {
            if(text.trim()!="")
            {
                
                commandLineLog.append("<font color=\"#FFFFFF\">"+text+"<\font>")
                root_window.evel(commandLineInput.text)
                if(root_window.getResult()!="")
                {
                    commandLineLog.append("<font color=\"#00FFFF\">"+root_window.getResult()+"<\font>")
                }
                
            }
            commandLineInput.text=""

        }
    }

    MouseArea{
        id:commandLineDragMouse
        anchors.fill: commandLineDrag
        drag.target: commandItem
        onPositionChanged: {
            if(parent.x<0)parent.x=0
            if(parent.y<0)parent.y=0
            if(parent.x>parent.parent.width-parent.width)parent.x=parent.parent.width-parent.width
            if(parent.y>parent.parent.height-parent.height)parent.y=parent.parent.height-parent.height
        }
    }


}

