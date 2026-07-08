import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtGraphicalEffects 1.15

Item {
    id: root
    property var backend: null
    property bool entered: false
    readonly property color accentColor: "#77F4FF"
    readonly property color accentStrong: "#63FFC7"
    readonly property color warmAccent: "#FFB26A"
    readonly property color metalLight: "#697687"
    readonly property color metalMid: "#445160"
    readonly property color metalDark: "#27313D"
    readonly property color metalDeep: "#121A22"
    readonly property color stageGlow: "#63E8FF"
    readonly property color stagePanel: "#101722"

    Component.onCompleted: entered = true

    component GlassPanel: Rectangle {
        radius: 26
        color: "#1B08111F"
        border.width: 1
        border.color: "#6DE6F3FF"
        opacity: root.entered ? 1 : 0
        y: root.entered ? 0 : 12

        Behavior on opacity { NumberAnimation { duration: 320 } }
        Behavior on y { NumberAnimation { duration: 320; easing.type: Easing.OutCubic } }

        layer.enabled: true
        layer.effect: DropShadow {
            color: "#68000000"
            radius: 24
            samples: 40
            verticalOffset: 10
        }

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#2EFCFFFF" }
                GradientStop { position: 0.42; color: "#18D9ECFF" }
                GradientStop { position: 1.0; color: "#07FFFFFF" }
            }
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: 1
            radius: parent.radius - 1
            color: "transparent"
            border.width: 1
            border.color: "#1FFFFFFF"
        }
    }

    component FingerGauge: Rectangle {
        id: gauge
        property string title: ""
        property int value: 0
        property color accent: root.accentColor

        radius: 18
        color: "#12081422"
        border.width: 1
        border.color: "#25FFFFFF"

        Column {
            anchors.centerIn: parent
            spacing: 4

            Rectangle {
                width: 42
                height: 4
                radius: 2
                color: gauge.accent
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: gauge.title
                color: "#DCEBFF"
                font.pixelSize: 14
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: gauge.value + "%"
                color: "#F9FCFF"
                font.pixelSize: 24
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    component FingerSegment: Item {
        id: seg
        property color segmentColor: root.metalMid
        property color accent: root.stageGlow
        property real segmentWidth: 24
        property real segmentHeight: 96
        property real glossStrength: 0.32

        width: segmentWidth + 18
        height: segmentHeight + 18

        Item {
            anchors.centerIn: parent
            width: seg.segmentWidth
            height: seg.segmentHeight

            layer.enabled: true
            layer.effect: DropShadow {
                color: "#5A06111A"
                radius: 12
                samples: 28
                verticalOffset: 6
            }

            Rectangle {
                anchors.fill: parent
                radius: width * 0.42
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.lighter(seg.segmentColor, 1.14) }
                    GradientStop { position: 0.18; color: Qt.lighter(seg.segmentColor, 1.04) }
                    GradientStop { position: 0.54; color: seg.segmentColor }
                    GradientStop { position: 1.0; color: Qt.darker(seg.segmentColor, 1.38) }
                }
                border.width: 1
                border.color: "#269FC8E4"
            }

            Rectangle {
                x: 4
                y: 5
                width: parent.width - 8
                height: parent.height * 0.3
                radius: width * 0.35
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#3EFFFFFF" }
                    GradientStop { position: 0.6; color: Qt.rgba(1, 1, 1, seg.glossStrength * 0.7) }
                    GradientStop { position: 1.0; color: "#00FFFFFF" }
                }
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width * 0.22
                radius: width * 0.4
                color: "#24000000"
            }

            Rectangle {
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 10
                width: 4
                height: parent.height - 20
                radius: 2
                color: "#22000000"
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 12
                width: 3
                height: parent.height - 24
                radius: 1.5
                color: seg.accent
                opacity: 0.82
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                width: parent.width * 0.55
                height: 5
                radius: 2.5
                color: "#12FFFFFF"
            }
        }
    }

    component JointOrb: Item {
        id: orb
        property real jointSize: 18
        property color jointColor: root.metalMid

        width: jointSize + 18
        height: jointSize + 18

        Item {
            anchors.centerIn: parent
            width: orb.jointSize
            height: orb.jointSize

            layer.enabled: true
            layer.effect: DropShadow {
                color: "#46000000"
                radius: 9
                samples: 20
                verticalOffset: 4
            }

            Rectangle {
                anchors.fill: parent
                radius: width / 2
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.lighter(orb.jointColor, 1.20) }
                    GradientStop { position: 0.22; color: Qt.lighter(orb.jointColor, 1.06) }
                    GradientStop { position: 1.0; color: Qt.darker(orb.jointColor, 1.36) }
                }
                border.width: 1
                border.color: "#2A89B8D4"
            }

            Rectangle {
                width: parent.width * 0.42
                height: width
                radius: width / 2
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.topMargin: 4
                color: "#2EFFFFFF"
            }
        }
    }

    component ArticulatedFinger: Item {
        id: finger
        property real percent: 0
        property color fingerColor: root.metalMid
        property color accentColor: root.stageGlow
        property real spread: 0
        property real proximalLength: 112
        property real middleLength: 82
        property real distalLength: 58
        property real proximalWidth: 26
        property real middleWidth: 24
        property real distalWidth: 22
        property bool thumb: false
        property real thumbBaseRotation: -42

        readonly property real p1: thumb ? percent * 0.32 : percent * 0.46
        readonly property real p2: thumb ? percent * 0.22 : percent * 0.28
        readonly property real p3: thumb ? percent * 0.16 : percent * 0.20
        property real baseYaw: thumb ? (thumbBaseRotation + percent * 0.08) : spread

        width: thumb ? 180 : 126
        height: thumb ? 236 : 338

        Behavior on baseYaw { NumberAnimation { duration: 240; easing.type: Easing.OutCubic } }

        Item {
            id: rootPivot
            width: 1
            height: 1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            rotation: finger.baseYaw

            Behavior on rotation { NumberAnimation { duration: 240; easing.type: Easing.OutCubic } }

            Item {
                id: hinge1
                width: 1
                height: 1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                rotation: finger.p1
                transformOrigin: Item.Bottom

                Behavior on rotation { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }

                FingerSegment {
                    id: seg1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    segmentColor: finger.fingerColor
                    accent: finger.accentColor
                    segmentWidth: finger.proximalWidth
                    segmentHeight: finger.proximalLength
                }

                JointOrb {
                    anchors.horizontalCenter: seg1.horizontalCenter
                    anchors.bottom: seg1.top
                    anchors.bottomMargin: -6
                    jointSize: finger.thumb ? 16 : 18
                    jointColor: "#A8BCD0"
                }

                Item {
                    id: hinge2
                    width: 1
                    height: 1
                    anchors.horizontalCenter: seg1.horizontalCenter
                    anchors.bottom: seg1.top
                    anchors.bottomMargin: 10
                    rotation: finger.p2
                    transformOrigin: Item.Bottom

                    Behavior on rotation { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }

                    FingerSegment {
                        id: seg2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        segmentColor: Qt.lighter(finger.fingerColor, 1.04)
                        accent: finger.accentColor
                        segmentWidth: finger.middleWidth
                        segmentHeight: finger.middleLength
                    }

                    JointOrb {
                        anchors.horizontalCenter: seg2.horizontalCenter
                        anchors.bottom: seg2.top
                        anchors.bottomMargin: -6
                        jointSize: finger.thumb ? 15 : 16
                        jointColor: "#9EB5CC"
                    }

                    Item {
                        id: hinge3
                        width: 1
                        height: 1
                        anchors.horizontalCenter: seg2.horizontalCenter
                        anchors.bottom: seg2.top
                        anchors.bottomMargin: 8
                        rotation: finger.p3
                        transformOrigin: Item.Bottom

                        Behavior on rotation { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }

                        FingerSegment {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottom: parent.bottom
                            segmentColor: Qt.lighter(finger.fingerColor, 1.08)
                            accent: finger.accentColor
                            segmentWidth: finger.distalWidth
                            segmentHeight: finger.distalLength
                        }
                    }
                }
            }
        }
    }

    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 20

        RowLayout {
            Layout.fillWidth: true
            spacing: 18

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                Text {
                    text: "自由控制实时姿态监测"
                    color: "#F9FCFF"
                    font.pixelSize: 42
                    font.bold: true
                }

                Text {
                    text: "沿用下位机全手控制逻辑，实时解析 Finger_All_Control_TASK 的姿态输出，显示五指大关节开合状态。"
                    color: "#E1EEFF"
                    font.pixelSize: 15
                    opacity: 0.95
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
            }

            Rectangle {
                Layout.preferredWidth: 196
                Layout.preferredHeight: 58
                radius: 20
                color: backend && backend.isConnected ? "#2238D8C4" : "#2A8F4343"
                border.width: 1
                border.color: backend && backend.isConnected ? "#CCF8FF" : "#FFD8D8"

                Row {
                    anchors.centerIn: parent
                    spacing: 10

                    Rectangle {
                        width: 11
                        height: 11
                        radius: 5.5
                        color: backend && backend.isConnected ? "#A7FFF4" : "#FFF0F0"
                    }

                    Text {
                        text: backend && backend.isConnected ? "姿态在线" : "姿态离线"
                        color: "#F9FCFF"
                        font.pixelSize: 17
                        font.bold: true
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 16

            GlassPanel {
                Layout.preferredWidth: 310
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 14

                    Text {
                        text: "连接与说明"
                        color: "#F8FBFF"
                        font.pixelSize: 28
                        font.bold: true
                    }

                    Text {
                        text: "该页不参与控制发送，只读取下位机全手控制任务的实时调试姿态。确认自由控制模式运行后，再看右侧手型变化。"
                        color: "#D7E7FB"
                        font.pixelSize: 13
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    Text {
                        text: "串口"
                        color: "#F3FAFF"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    ComboBox {
                        id: portBox
                        Layout.fillWidth: true
                        model: backend ? backend.availablePorts : []
                        Material.background: "#24071422"
                        Material.foreground: "#F6FBFF"
                    }

                    Text {
                        text: "波特率"
                        color: "#F3FAFF"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    ComboBox {
                        id: baudBox
                        Layout.fillWidth: true
                        model: ["115200", "1000000"]
                        currentIndex: 0
                        Material.background: "#24071422"
                        Material.foreground: "#F6FBFF"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Button {
                            Layout.fillWidth: true
                            text: "刷新串口"
                            onClicked: if (backend) backend.refreshPorts()
                        }

                        Button {
                            Layout.fillWidth: true
                            text: backend && backend.isConnected ? "断开连接" : "建立连接"
                            onClicked: {
                                if (!backend)
                                    return
                                if (backend.isConnected)
                                    backend.disconnectDevice()
                                else if (portBox.currentText !== "")
                                    backend.connectDevice(portBox.currentText, parseInt(baudBox.currentText))
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 52
                        radius: 14
                        color: backend && backend.isConnected ? "#1C1D8770" : "#2A6E2A2A"
                        border.width: 1
                        border.color: backend && backend.isConnected ? "#74F4E2" : "#F1B1B1"

                        Text {
                            anchors.fill: parent
                            anchors.margins: 12
                            verticalAlignment: Text.AlignVCenter
                            text: backend ? backend.connectionMessage : "未初始化"
                            color: "#F9FCFF"
                            font.pixelSize: 13
                            wrapMode: Text.WordWrap
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: "#35FFFFFF"
                    }

                    Text {
                        text: "当前姿态"
                        color: "#F4FAFF"
                        font.pixelSize: 18
                        font.bold: true
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 10
                        rowSpacing: 10

                        FingerGauge { Layout.fillWidth: true; Layout.preferredHeight: 98; title: "拇指"; value: backend ? backend.thumbPos : 0; accent: "#7EF9FF" }
                        FingerGauge { Layout.fillWidth: true; Layout.preferredHeight: 98; title: "食指"; value: backend ? backend.indexPos : 0; accent: "#6DFFD5" }
                        FingerGauge { Layout.fillWidth: true; Layout.preferredHeight: 98; title: "中指"; value: backend ? backend.middlePos : 0; accent: "#7BC4FF" }
                        FingerGauge { Layout.fillWidth: true; Layout.preferredHeight: 98; title: "无名指"; value: backend ? backend.ringPos : 0; accent: "#B790FF" }
                        FingerGauge { Layout.fillWidth: true; Layout.preferredHeight: 98; title: "小指"; value: backend ? backend.pinkyPos : 0; accent: "#FFB26A" }
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 16

                GlassPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 18
                        spacing: 14

                        RowLayout {
                            Layout.fillWidth: true

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4

                                Text {
                                    text: "手部实时姿态"
                                    color: "#F9FBFF"
                                    font.pixelSize: 30
                                    font.bold: true
                                }

                                Text {
                                    text: "采用五指大关节开合百分比驱动可视化手型。姿态来源于下位机控制任务实时输出，而不是上位机再控制一次。"
                                    color: "#DDEBFF"
                                    font.pixelSize: 13
                                    wrapMode: Text.WordWrap
                                    Layout.fillWidth: true
                                }
                            }

                            Rectangle {
                                Layout.preferredWidth: 150
                                Layout.preferredHeight: 56
                                radius: 18
                                color: "#1A0C1622"
                                border.width: 1
                                border.color: "#44DAF6FF"

                                Column {
                                    anchors.centerIn: parent
                                    spacing: 2

                                    Text {
                                        text: "整体开合"
                                        color: "#BFE5FF"
                                        font.pixelSize: 12
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }

                                    Text {
                                        text: Math.round(((backend ? backend.thumbPos : 0)
                                                           + (backend ? backend.indexPos : 0)
                                                           + (backend ? backend.middlePos : 0)
                                                           + (backend ? backend.ringPos : 0)
                                                           + (backend ? backend.pinkyPos : 0)) / 5) + "%"
                                        color: "#F9FCFF"
                                        font.pixelSize: 22
                                        font.bold: true
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            Rectangle {
                                anchors.centerIn: parent
                                width: 620
                                height: 452
                                radius: 40
                                color: "#100B1220"
                                border.width: 1
                                border.color: "#2ADAF6FF"
                            }

                            Rectangle {
                                anchors.centerIn: parent
                                width: 620
                                height: 452
                                radius: 40
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: "#123D5470" }
                                    GradientStop { position: 0.38; color: "#0F19293B" }
                                    GradientStop { position: 1.0; color: "#10131A28" }
                                }
                            }

                            Repeater {
                                model: 9

                                delegate: Rectangle {
                                    width: 560
                                    height: 1
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    y: 58 + index * 42
                                    color: index % 2 === 0 ? "#0E8DC2E0" : "#092A435B"
                                    opacity: 0.45
                                }
                            }

                            Repeater {
                                model: 11

                                delegate: Rectangle {
                                    width: 1
                                    height: 370
                                    anchors.top: parent.top
                                    anchors.topMargin: 42
                                    x: 48 + index * 52
                                    color: index % 2 === 0 ? "#0D86D0F2" : "#071F374D"
                                    opacity: 0.32
                                }
                            }

                            Rectangle {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 36
                                width: 380
                                height: 84
                                radius: 42
                                    color: "#2C79D9FF"
                                    opacity: 0.12

                                    layer.enabled: true
                                    layer.effect: FastBlur {
                                    radius: 54
                                    transparentBorder: true
                                }
                            }

                            Item {
                                id: handScene
                                width: 580
                                height: 410
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 10

                                Rectangle {
                                    width: 420
                                    height: 60
                                    radius: 30
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 26
                                    color: "#63030B15"

                                    layer.enabled: true
                                    layer.effect: FastBlur {
                                        radius: 36
                                        transparentBorder: true
                                    }
                                }

                                Rectangle {
                                    width: 286
                                    height: 28
                                    radius: 14
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 64
                                    color: "#14000000"
                                    border.width: 1
                                    border.color: "#14FFFFFF"
                                }

                                Item {
                                    id: handRig
                                    width: 420
                                    height: 360
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 42

                                    Item {
                                        id: wristUnit
                                        width: 148
                                        height: 140
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.bottom: parent.bottom

                                        layer.enabled: true
                                        layer.effect: DropShadow {
                                            color: "#76000000"
                                            radius: 24
                                            samples: 40
                                            verticalOffset: 16
                                        }

                                        Rectangle {
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.bottom: parent.bottom
                                            width: 136
                                            height: 116
                                            radius: 46
                                            gradient: Gradient {
                                                GradientStop { position: 0.0; color: "#8FA2B7" }
                                                GradientStop { position: 0.55; color: "#66788D" }
                                                GradientStop { position: 1.0; color: "#415060" }
                                            }
                                            border.width: 1
                                            border.color: "#18FFFFFF"
                                        }

                                        Rectangle {
                                            anchors.top: parent.top
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            width: 110
                                            height: 34
                                            radius: 17
                                            gradient: Gradient {
                                                GradientStop { position: 0.0; color: "#3DFFFFFF" }
                                                GradientStop { position: 1.0; color: "#00000000" }
                                            }
                                        }
                                    }

                                    Item {
                                        id: palmUnit
                                        width: 270
                                        height: 246
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.bottom: wristUnit.top
                                        anchors.bottomMargin: -32

                                        layer.enabled: true
                                        layer.effect: DropShadow {
                                            color: "#83000000"
                                            radius: 28
                                            samples: 48
                                            verticalOffset: 16
                                        }

                                        Rectangle {
                                            id: palmShell
                                            anchors.fill: parent
                                            radius: 98
                                            gradient: Gradient {
                                                GradientStop { position: 0.0; color: "#D7E2ED" }
                                                GradientStop { position: 0.18; color: "#BACADA" }
                                                GradientStop { position: 0.52; color: "#8CA0B4" }
                                                GradientStop { position: 1.0; color: "#617383" }
                                            }
                                            border.width: 1
                                            border.color: "#24FFFFFF"
                                        }

                                        Rectangle {
                                            anchors.top: parent.top
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.topMargin: 16
                                            width: 214
                                            height: 56
                                            radius: 28
                                            gradient: Gradient {
                                                GradientStop { position: 0.0; color: "#5BFFFFFF" }
                                                GradientStop { position: 0.56; color: "#18FFFFFF" }
                                                GradientStop { position: 1.0; color: "#00FFFFFF" }
                                            }
                                        }

                                        Rectangle {
                                            width: 108
                                            height: 132
                                            radius: 50
                                            x: 30
                                            y: 92
                                            rotation: -18
                                            gradient: Gradient {
                                                GradientStop { position: 0.0; color: "#B1C1D0" }
                                                GradientStop { position: 1.0; color: "#73879A" }
                                            }
                                            opacity: 0.88
                                        }

                                        Rectangle {
                                            width: 98
                                            height: 18
                                            radius: 9
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.bottom: parent.bottom
                                            anchors.bottomMargin: 30
                                            color: "#15000000"
                                        }
                                    }

                                    Repeater {
                                        model: 4

                                        delegate: Rectangle {
                                            width: 18
                                            height: 56
                                            radius: 9
                                            anchors.bottom: palmUnit.top
                                            anchors.bottomMargin: -10
                                            anchors.horizontalCenter: palmUnit.horizontalCenter
                                            anchors.horizontalCenterOffset: -92 + index * 61
                                            color: "#2CFFFFFF"
                                        }
                                    }

                                    ArticulatedFinger {
                                        anchors.bottom: palmUnit.top
                                        anchors.bottomMargin: -10
                                        anchors.horizontalCenter: palmUnit.horizontalCenter
                                        anchors.horizontalCenterOffset: -106
                                        percent: backend ? backend.indexPos : 0
                                        fingerColor: "#97AABF"
                                        accentColor: "#7EEBFF"
                                        spread: -15
                                        proximalLength: 118
                                        middleLength: 88
                                        distalLength: 62
                                    }

                                    ArticulatedFinger {
                                        anchors.bottom: palmUnit.top
                                        anchors.bottomMargin: -12
                                        anchors.horizontalCenter: palmUnit.horizontalCenter
                                        anchors.horizontalCenterOffset: -34
                                        percent: backend ? backend.middlePos : 0
                                        fingerColor: "#9AAFC2"
                                        accentColor: "#74FFD1"
                                        spread: -5
                                        proximalLength: 132
                                        middleLength: 96
                                        distalLength: 66
                                        proximalWidth: 28
                                        middleWidth: 26
                                    }

                                    ArticulatedFinger {
                                        anchors.bottom: palmUnit.top
                                        anchors.bottomMargin: -10
                                        anchors.horizontalCenter: palmUnit.horizontalCenter
                                        anchors.horizontalCenterOffset: 38
                                        percent: backend ? backend.ringPos : 0
                                        fingerColor: "#95AAC0"
                                        accentColor: "#81BFFF"
                                        spread: 6
                                        proximalLength: 126
                                        middleLength: 90
                                        distalLength: 64
                                    }

                                    ArticulatedFinger {
                                        anchors.bottom: palmUnit.top
                                        anchors.bottomMargin: -6
                                        anchors.horizontalCenter: palmUnit.horizontalCenter
                                        anchors.horizontalCenterOffset: 108
                                        percent: backend ? backend.pinkyPos : 0
                                        fingerColor: "#A49DB9"
                                        accentColor: "#CBA9FF"
                                        spread: 16
                                        proximalLength: 102
                                        middleLength: 78
                                        distalLength: 56
                                        proximalWidth: 23
                                        middleWidth: 21
                                        distalWidth: 19
                                    }

                                    ArticulatedFinger {
                                        anchors.left: palmUnit.left
                                        anchors.leftMargin: -92
                                        anchors.bottom: palmUnit.bottom
                                        anchors.bottomMargin: 14
                                        percent: backend ? backend.thumbPos : 0
                                        fingerColor: "#B49E8A"
                                        accentColor: "#FFBE86"
                                        thumb: true
                                        thumbBaseRotation: -44
                                        proximalLength: 90
                                        middleLength: 68
                                        distalLength: 48
                                        proximalWidth: 30
                                        middleWidth: 27
                                        distalWidth: 24
                                    }

                                    Rectangle {
                                        anchors.left: palmUnit.right
                                        anchors.leftMargin: 18
                                        anchors.top: palmUnit.top
                                        anchors.topMargin: 18
                                        width: 98
                                        height: 98
                                        radius: 28
                                        color: "#1A0E1724"
                                        border.width: 1
                                        border.color: "#26FFFFFF"

                                        Column {
                                            anchors.centerIn: parent
                                            spacing: 4

                                            Text {
                                                text: "姿态状态"
                                                color: "#CFE6FF"
                                                font.pixelSize: 12
                                                anchors.horizontalCenter: parent.horizontalCenter
                                            }

                                            Text {
                                                text: Math.round(((backend ? backend.thumbPos : 0)
                                                                   + (backend ? backend.indexPos : 0)
                                                                   + (backend ? backend.middlePos : 0)
                                                                   + (backend ? backend.ringPos : 0)
                                                                   + (backend ? backend.pinkyPos : 0)) / 5)
                                                color: "#F9FCFF"
                                                font.pixelSize: 30
                                                font.bold: true
                                                anchors.horizontalCenter: parent.horizontalCenter
                                            }

                                            Text {
                                                text: "%"
                                                color: "#9ACDFF"
                                                font.pixelSize: 13
                                                anchors.horizontalCenter: parent.horizontalCenter
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
