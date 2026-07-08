import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtGraphicalEffects 1.15

ApplicationWindow {
    id: appWindow
    visible: true
    width: 1360
    height: 820
    title: "仿生手康复训练系统"
    property int currentMode: 0

    // Material 风格主题
    Material.theme: Material.Dark
    Material.accent: Material.Cyan
    Material.primary: Material.BlueGrey

    Item {
        anchors.fill: parent

        // 动态背景层
        Item {
            id: backgroundScene
            anchors.fill: parent

            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#10294D" }
                    GradientStop { position: 0.32; color: "#273C77" }
                    GradientStop { position: 0.68; color: "#4E4E8A" }
                    GradientStop { position: 1.0; color: "#8F6244" }
                }
            }

            Rectangle {
                width: 620
                height: 620
                radius: 310
                x: -120
                y: -100
                color: "#72E7FF"
                opacity: 0.22

                SequentialAnimation on x {
                    loops: Animation.Infinite
                    NumberAnimation { to: 120; duration: 11000; easing.type: Easing.InOutSine }
                    NumberAnimation { to: -120; duration: 11000; easing.type: Easing.InOutSine }
                }
            }

            Rectangle {
                width: 460
                height: 460
                radius: 230
                x: appWindow.width - width - 80
                y: appWindow.height - height - 30
                color: "#FF82C8"
                opacity: 0.18

                SequentialAnimation on y {
                    loops: Animation.Infinite
                    NumberAnimation { to: appWindow.height - height - 210; duration: 7600; easing.type: Easing.InOutSine }
                    NumberAnimation { to: appWindow.height - height - 30; duration: 7600; easing.type: Easing.InOutSine }
                }
            }

            Rectangle {
                width: 320
                height: 320
                radius: 160
                x: appWindow.width * 0.42
                y: appWindow.height * 0.08
                color: "#A08EFF"
                opacity: 0.12

                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    NumberAnimation { from: 0.08; to: 0.18; duration: 5200; easing.type: Easing.InOutSine }
                    NumberAnimation { from: 0.18; to: 0.08; duration: 5200; easing.type: Easing.InOutSine }
                }
            }
        }

        // 全局毛玻璃模糊层（对整个背景场景做实时模糊）
        ShaderEffectSource {
            id: blurSource
            anchors.fill: parent
            sourceItem: backgroundScene
            live: true
            hideSource: false
        }

        FastBlur {
            anchors.fill: parent
            source: blurSource
            radius: 72
            transparentBorder: true
        }

        // 第一层：深色染底，拉高前景对比
        Rectangle {
            anchors.fill: parent
            color: "#58101B2D"
        }

        // 第二层：玻璃雾面高光
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#42FFFFFF" }
                GradientStop { position: 0.28; color: "#16FFFFFF" }
                GradientStop { position: 1.0; color: "#06FFFFFF" }
            }
        }

        // 第三层：冷暖统一色调
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#1E2E5E99" }
                GradientStop { position: 0.55; color: "#0C152E55" }
                GradientStop { position: 1.0; color: "#24694224" }
            }
        }

        // 第四层：轻微暗角，避免内容发飘
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "#36000000" }
                GradientStop { position: 0.18; color: "#00000000" }
                GradientStop { position: 0.82; color: "#00000000" }
                GradientStop { position: 1.0; color: "#36000000" }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.width: 1
            border.color: "#18FFFFFF"
            opacity: 0.45
        }

        Item {
            anchors.fill: parent

            Loader {
                anchors.fill: parent
                sourceComponent: appWindow.currentMode === 0 ? freeControlPage : rehabPage
            }

            Rectangle {
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 20
                width: 290
                height: 54
                radius: 20
                color: "#220C1727"
                border.width: 1
                border.color: "#5EDBF4FF"

                Row {
                    anchors.fill: parent
                    anchors.margins: 6
                    spacing: 6

                    Repeater {
                        model: [
                            { label: "自由控制", index: 0 },
                            { label: "发力评估", index: 1 }
                        ]

                        delegate: Rectangle {
                            width: 136
                            height: 42
                            radius: 16
                            color: appWindow.currentMode === modelData.index ? "#35D9F7FF" : "transparent"
                            border.width: appWindow.currentMode === modelData.index ? 1 : 0
                            border.color: "#CFFBFF"

                            Text {
                                anchors.centerIn: parent
                                text: modelData.label
                                color: appWindow.currentMode === modelData.index ? "#08131F" : "#ECF8FF"
                                font.pixelSize: 15
                                font.bold: true
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: appWindow.currentMode = modelData.index
                            }
                        }
                    }
                }
            }
        }

        Component {
            id: freeControlPage
            FreeControl {
                anchors.fill: parent
                backend: SerialBackend
            }
        }

        Component {
            id: rehabPage
            RehabMonitor {
                anchors.fill: parent
                backend: SerialBackend
            }
        }
    }
}
