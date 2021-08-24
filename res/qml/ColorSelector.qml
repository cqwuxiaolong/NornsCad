import QtWebEngine 1.8
import QtWebChannel 1.0
import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Window 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Item {
    id:colorSelectorItem
    width: 210
    height: width

    
    WebChannel{
        id:webChannel
    }

    QtObject {
        id: myObject
        objectName: "myObject"

        // the identifier under which this object
        // will be known on the JavaScript side
        //WebChannel.id: "webChannel"

        property var send: function (arg) {
                    sendTextMessage(arg);
                }

        // signals, methods and properties are
        // accessible to JavaScript code
        signal someSignal(string message)


        function someMethod(message) {
            //console.log(message);
            viewer.occtColor=message
            //someSignal(message);
            // return dataManager.getGeoLat();
        }

        property string hello: "world";
    }

    WebEngineView{
        anchors.fill: parent;
        backgroundColor: "#00000000"
        url:"qrc:/ColorPicker.html"
        webChannel: webChannel
        onContextMenuRequested: {
                request.accepted = true
            }
        
    }

    Component.onCompleted: {
        webChannel.registerObject("foo", myObject);
        //Expose C++ object
        // webChannel.registerObject("bar", dataManager);
    }
    
    ToolButton {
                    id:colordrag
                    text: qsTr("Drag")
                    checkable: true
                    hoverEnabled:true
                    display: AbstractButton.IconOnly
                    icon {source:"qrc:/drag.png"
                            color: "white"}
                    ToolTip{
                        text: qsTr("Drag")
                        visible: parent.hovered
                        y:parent.height
                    }
                    MouseArea{
                        anchors.fill: parent
                        drag.target: parent.parent.parent.parent
                        hoverEnabled: true
                        onPositionChanged: {
                            if(toolbar.x<0)toolbar.x=0
                            if(toolbar.y<0)toolbar.y=0
                            if(toolbar.x>toolbar.parent.width-parent.width)toolbar.x=toolbar.parent.width-parent.width
                            if(toolbar.y>toolbar.parent.height-parent.height)toolbar.y=toolbar.parent.height-parent.height
                        }
                    }
                }
}
