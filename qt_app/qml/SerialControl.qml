import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: serialControlPage
    property var backend: null
    property bool entered: false
    Component.onCompleted: entered = true

    Column {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 16

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "设备连接"
            color: "#F8FCFF"
            font.pixelSize: 42
            font.bold: true
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "仅用于串口连接与通信状态"
            color: "#E1EEFF"
            font.pixelSize: 16
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        Rectangle {
            width: 560
            height: 260
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 22
            color: "#28FFFFFF"
            border.width: 1
            border.color: "#90FFFFFF"
            opacity: serialControlPage.entered ? 1 : 0
            y: serialControlPage.entered ? 0 : 16

            Behavior on opacity { NumberAnimation { duration: 300 } }
            Behavior on y { NumberAnimation { duration: 300; easing.type: Easing.OutCubic } }

            Rectangle {
                anchors.fill: parent
                radius: parent.radius
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#36FFFFFF" }
                    GradientStop { position: 1.0; color: "#0CFFFFFF" }
                }
            }

            Column {
                anchors.fill: parent
                anchors.margins: 22
                spacing: 14

                Row {
                    spacing: 10
                    Text { text: "串口"; color: "#F4FAFF"; font.pixelSize: 16; width: 60 }
                    ComboBox {
                        id: portBox
                        width: 430
                        model: backend ? backend.availablePorts : []
                    }
                }

                Row {
                    spacing: 10
                    Text { text: "波特率"; color: "#F4FAFF"; font.pixelSize: 16; width: 60 }
                    ComboBox {
                        id: baudBox
                        width: 430
                        model: ["115200", "1000000"]
                        currentIndex: 0
                    }
                }

                Row {
                    spacing: 10
                    Button { text: "刷新串口"; onClicked: if (backend) backend.refreshPorts() }
                    Button {
                        text: (backend && backend.isConnected) ? "断开连接" : "建立连接"
                        onClicked: {
                            if (!backend) return
                            if (backend.isConnected) backend.disconnectDevice()
                            else if (portBox.currentText !== "") backend.connectDevice(portBox.currentText, parseInt(baudBox.currentText))
                        }
                    }
                }

                Text {
                    text: (backend && backend.isConnected) ? "状态: 已连接" : "状态: 未连接"
                    color: (backend && backend.isConnected) ? "#D2FFE4" : "#FFE1E1"
                    font.pixelSize: 15
                }
            }
        }
    }
}
