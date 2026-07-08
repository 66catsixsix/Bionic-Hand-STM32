import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtGraphicalEffects 1.15

Item {
    id: root
    property var backend: null
    property bool entered: false
    readonly property int currentLevel: backend ? backend.emgLevel : 0
    readonly property color idleColor: "#7E8EA8"
    readonly property color effectiveColor: "#4BE2B9"
    readonly property color strongColor: "#FF9852"
    readonly property color chartColor: "#87F7FF"
    readonly property color levelAccent: currentLevel >= (backend ? backend.emgStrongThreshold : 75)
                                        ? strongColor
                                        : (currentLevel >= (backend ? backend.emgEffectiveThreshold : 45) ? effectiveColor : chartColor)
    readonly property color levelAura: currentLevel >= (backend ? backend.emgStrongThreshold : 75)
                                      ? "#66FF9852"
                                      : (currentLevel >= (backend ? backend.emgEffectiveThreshold : 45) ? "#664BE2B9" : "#6687F7FF")

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

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 2
            height: parent.height * 0.3
            radius: parent.radius - 2
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#36FFFFFF" }
                GradientStop { position: 1.0; color: "#00FFFFFF" }
            }
        }

        Rectangle {
            width: parent.width * 0.28
            height: parent.width * 0.28
            radius: width / 2
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: -width * 0.18
            anchors.topMargin: -height * 0.18
            color: "#14B4EFFF"
        }
    }

    component AccentBadge: Rectangle {
        property string badgeText: ""
        radius: 12
        color: "#1A89CFFF"
        border.width: 1
        border.color: "#58DFFBFF"
        implicitWidth: badgeLabel.implicitWidth + 18
        implicitHeight: badgeLabel.implicitHeight + 10

        Text {
            id: badgeLabel
            anchors.centerIn: parent
            text: parent.badgeText
            color: "#E9FBFF"
            font.pixelSize: 11
            font.bold: true
            font.letterSpacing: 0.8
        }
    }

    Connections {
        target: backend
        function onEmgHistoryChanged() {
            lineCanvas.requestPaint()
            pieCanvas.requestPaint()
        }
        function onEmgMetricsChanged() {
            lineCanvas.requestPaint()
            pieCanvas.requestPaint()
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
                    text: "EMG 发力评估工作站"
                    color: "#F9FCFF"
                    font.pixelSize: 42
                    font.bold: true
                    font.letterSpacing: 0.5
                }

                Text {
                    text: "单页实时监测前臂肌电发力状态，自动统计无发力、有效发力与强发力占比"
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

                        SequentialAnimation on opacity {
                            running: backend && backend.isConnected
                            loops: Animation.Infinite
                            NumberAnimation { from: 1.0; to: 0.25; duration: 650 }
                            NumberAnimation { from: 0.25; to: 1.0; duration: 650 }
                        }
                    }

                    Text {
                        text: backend && backend.isConnected ? "串口在线" : "串口离线"
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
                Layout.preferredWidth: 292
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 14

                    Text {
                        text: "设备与测试"
                        color: "#F8FBFF"
                        font.pixelSize: 28
                        font.bold: true
                    }

                    AccentBadge {
                        badgeText: "ACQUISITION DECK"
                    }

                    Text {
                        text: "在这里完成串口连接、会话重置和阈值理解，评估页不再单独切换。"
                        color: "#D7E7FB"
                        font.pixelSize: 13
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 10

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
                                Material.background: "#2A12304B"
                                Material.foreground: "#F7FBFF"
                                onClicked: if (backend) backend.refreshPorts()
                            }

                            Button {
                                Layout.fillWidth: true
                                text: (backend && backend.isConnected) ? "断开连接" : "建立连接"
                                Material.background: backend && backend.isConnected ? "#345A2933" : "#2F0F4E6A"
                                Material.foreground: "#F9FCFF"
                                onClicked: {
                                    if (!backend)
                                        return
                                    if (backend.isConnected) {
                                        backend.disconnectDevice()
                                    } else if (portBox.currentText !== "") {
                                        backend.connectDevice(portBox.currentText, parseInt(baudBox.currentText))
                                    }
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
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: "#35FFFFFF"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        Rectangle {
                            Layout.preferredWidth: 112
                            Layout.preferredHeight: 112
                            radius: 56
                            color: "#1C07141F"
                            border.width: 2
                            border.color: root.levelAccent

                            Rectangle {
                                anchors.centerIn: parent
                                width: 124
                                height: 124
                                radius: 62
                                color: root.levelAura
                                opacity: 0.22
                                scale: 0.9

                                SequentialAnimation on scale {
                                    running: true
                                    loops: Animation.Infinite
                                    NumberAnimation { from: 0.9; to: 1.04; duration: 1200; easing.type: Easing.OutQuad }
                                    NumberAnimation { from: 1.04; to: 0.9; duration: 1200; easing.type: Easing.InQuad }
                                }
                            }

                            Rectangle {
                                anchors.centerIn: parent
                                width: 84
                                height: 84
                                radius: 42
                                color: "#32081422"
                            }

                            Column {
                                anchors.centerIn: parent
                                spacing: 0

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: root.currentLevel
                                    color: "#FAFDFF"
                                    font.pixelSize: 32
                                    font.bold: true
                                }

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "lvl"
                                    color: "#BCD7EE"
                                    font.pixelSize: 12
                                    font.bold: true
                                }
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 6

                            Text {
                                text: backend ? backend.emgStateText : "无数据"
                                color: "#FAFDFF"
                                font.pixelSize: 26
                                font.bold: true
                            }

                            Text {
                                text: "avg " + (backend ? backend.emgAverage : 0)
                                      + "   act " + (backend ? backend.emgActivity : 0)
                                      + "   delta " + (backend ? backend.emgDelta : 0)
                                color: "#D7E6FA"
                                font.pixelSize: 13
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Text {
                                text: "有效发力 ≥ " + (backend ? backend.emgEffectiveThreshold : 50)
                                      + "，强发力 ≥ " + (backend ? backend.emgStrongThreshold : 80)
                                color: "#D7E6FA"
                                font.pixelSize: 13
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Button {
                                Layout.preferredWidth: 130
                                text: "重置本次评估"
                                enabled: backend !== null
                                Material.background: "#2A153754"
                                Material.foreground: "#F8FCFF"
                                onClicked: if (backend) backend.resetEmgSession()
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: "#35FFFFFF"
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Text {
                            text: "测试建议"
                            color: "#F4FAFF"
                            font.pixelSize: 17
                            font.bold: true
                        }

                        Repeater {
                            model: [
                                "先放松 3 秒，再中等发力 3 秒，再明显发力 3 秒。",
                                "持续握到疼没有必要，更适合短时重复发力评估。",
                                "优先看有效发力占比和强发力占比，不要只盯瞬时值。"
                            ]

                            delegate: RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                Rectangle {
                                    Layout.preferredWidth: 6
                                    Layout.preferredHeight: 6
                                    radius: 3
                                    color: index === 0 ? root.chartColor : (index === 1 ? root.effectiveColor : root.strongColor)
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: modelData
                                    color: "#D9E7FB"
                                    font.pixelSize: 12
                                    wrapMode: Text.WordWrap
                                }
                            }
                        }
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
                    Layout.preferredHeight: 326

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 18
                        spacing: 12

                        RowLayout {
                            Layout.fillWidth: true

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4

                                Text {
                                    text: "实时发力折线"
                                    color: "#F9FBFF"
                                    font.pixelSize: 30
                                    font.bold: true
                                }

                                AccentBadge {
                                    badgeText: "LIVE MUSCLE INTENSITY"
                                }

                                Text {
                                    text: "主显示最近 180 个 lvl 采样点，适合观察发力节奏与波动。"
                                    color: "#DDEBFF"
                                    font.pixelSize: 13
                                }
                            }

                            ColumnLayout {
                                spacing: 2

                                Text {
                                    text: "LIVE " + root.currentLevel
                                    color: root.levelAccent
                                    font.pixelSize: 16
                                    font.bold: true
                                    horizontalAlignment: Text.AlignRight
                                }

                                Text {
                                    text: "PEAK " + (backend ? backend.emgPeakLevel : 0)
                                    color: "#F9FCFF"
                                    font.pixelSize: 18
                                    font.bold: true
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            radius: 18
                            color: "#1207141F"
                            border.width: 1
                            border.color: "#52BDF5FF"

                            Rectangle {
                                anchors.fill: parent
                                radius: parent.radius
                                gradient: Gradient {
                                    orientation: Gradient.Horizontal
                                    GradientStop { position: 0.0; color: "#0F60D9FF" }
                                    GradientStop { position: 0.55; color: "#0829565A" }
                                    GradientStop { position: 1.0; color: "#0AFFA04D" }
                                }
                                opacity: 0.23
                            }

                            Canvas {
                                id: lineCanvas
                                anchors.fill: parent
                                anchors.margins: 10

                                onPaint: {
                                    var ctx = getContext("2d")
                                    ctx.reset()

                                    var w = width
                                    var h = height
                                    var history = backend ? backend.emgLevelHistory : []

                                    ctx.strokeStyle = "rgba(255,255,255,0.10)"
                                    ctx.lineWidth = 1
                                    for (var i = 0; i <= 4; ++i) {
                                        var gy = i * h / 4
                                        ctx.beginPath()
                                        ctx.moveTo(0, gy)
                                        ctx.lineTo(w, gy)
                                        ctx.stroke()
                                    }

                                    if (!history || history.length < 2)
                                        return

                                    var fill = ctx.createLinearGradient(0, 0, 0, h)
                                    fill.addColorStop(0.0, "rgba(135,247,255,0.28)")
                                    fill.addColorStop(1.0, "rgba(135,247,255,0.02)")

                                    ctx.beginPath()
                                    for (var p = 0; p < history.length; ++p) {
                                        var x = (history.length === 1) ? 0 : (p * w / (history.length - 1))
                                        var y = h - (Number(history[p]) / 100.0) * h
                                        if (p === 0)
                                            ctx.moveTo(x, y)
                                        else
                                            ctx.lineTo(x, y)
                                    }
                                    ctx.lineTo(w, h)
                                    ctx.lineTo(0, h)
                                    ctx.closePath()
                                    ctx.fillStyle = fill
                                    ctx.fill()

                                    var stroke = ctx.createLinearGradient(0, 0, w, 0)
                                    stroke.addColorStop(0.0, "#87F7FF")
                                    stroke.addColorStop(0.5, "#5CFFD0")
                                    stroke.addColorStop(1.0, "#FFD76B")

                                    ctx.beginPath()
                                    for (var j = 0; j < history.length; ++j) {
                                        var xx = (history.length === 1) ? 0 : (j * w / (history.length - 1))
                                        var yy = h - (Number(history[j]) / 100.0) * h
                                        if (j === 0)
                                            ctx.moveTo(xx, yy)
                                        else
                                            ctx.lineTo(xx, yy)
                                    }
                                    ctx.strokeStyle = stroke
                                    ctx.lineWidth = 3
                                    ctx.lineJoin = "round"
                                    ctx.lineCap = "round"
                                    ctx.stroke()
                                }
                            }

                            Column {
                                anchors.left: parent.left
                                anchors.leftMargin: 12
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 42

                                Repeater {
                                    model: ["100", "75", "50", "25", "0"]
                                    delegate: Text {
                                        text: modelData
                                        color: "#B5C9E6"
                                        font.pixelSize: 11
                                    }
                                }
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 16

                    GlassPanel {
                        Layout.preferredWidth: 330
                        Layout.fillHeight: true

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 18
                            spacing: 12

                            Text {
                                text: "发力构成"
                                color: "#F7FBFF"
                                font.pixelSize: 28
                                font.bold: true
                            }

                            AccentBadge {
                                badgeText: "SESSION COMPOSITION"
                            }

                            Text {
                                text: "扇形图直观展示当前会话中无发力、有效发力和强发力的时间占比。"
                                color: "#D7E6FA"
                                font.pixelSize: 13
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                Canvas {
                                    id: pieCanvas
                                    width: 220
                                    height: 220
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.top: parent.top

                                    onPaint: {
                                        var ctx = getContext("2d")
                                        ctx.reset()

                                        var cx = width / 2
                                        var cy = height / 2
                                        var r = Math.min(width, height) / 2 - 12
                                        var line = 28
                                        var start = -Math.PI / 2

                                        var idle = backend ? backend.emgIdleRatio : 0
                                        var effective = backend ? backend.emgEffectiveRatio : 0
                                        var strong = backend ? backend.emgStrongRatio : 0
                                        var values = [idle, effective, strong]
                                        var colors = [root.idleColor, root.effectiveColor, root.strongColor]

                                        for (var i = 0; i < values.length; ++i) {
                                            var sweep = (values[i] / 100.0) * Math.PI * 2
                                            if (sweep <= 0)
                                                continue
                                            ctx.beginPath()
                                            ctx.arc(cx, cy, r, start, start + sweep, false)
                                            ctx.strokeStyle = colors[i]
                                            ctx.lineWidth = line
                                            ctx.stroke()
                                            start += sweep
                                        }

                                        ctx.beginPath()
                                        ctx.arc(cx, cy, r - line, 0, Math.PI * 2)
                                        ctx.fillStyle = "#22091727"
                                        ctx.fill()

                                        ctx.fillStyle = "#F9FCFF"
                                        ctx.font = "bold 24px sans-serif"
                                        ctx.textAlign = "center"
                                        ctx.fillText((backend ? backend.emgPeakLevel : 0).toString(), cx, cy - 2)
                                        ctx.font = "12px sans-serif"
                                        ctx.fillStyle = "#BFD5EC"
                                        ctx.fillText("PEAK", cx, cy + 20)
                                    }
                                }

                                Column {
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 6
                                    spacing: 10

                                    Repeater {
                                        model: [
                                            { label: "无发力", color: root.idleColor, value: backend ? backend.emgIdleRatio : 0 },
                                            { label: "有效发力", color: root.effectiveColor, value: backend ? backend.emgEffectiveRatio : 0 },
                                            { label: "强发力", color: root.strongColor, value: backend ? backend.emgStrongRatio : 0 }
                                        ]

                                        delegate: Row {
                                            spacing: 8

                                            Rectangle {
                                                width: 12
                                                height: 12
                                                radius: 6
                                                color: modelData.color
                                                anchors.verticalCenter: parent.verticalCenter
                                            }

                                            Text {
                                                text: modelData.label + "  " + Number(modelData.value).toFixed(1) + "%"
                                                color: "#E6F0FB"
                                                font.pixelSize: 14
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columns: 2
                        columnSpacing: 14
                        rowSpacing: 14

                        Repeater {
                            model: [
                                { title: "峰值发力", value: backend ? backend.emgPeakLevel : 0, unit: "lvl", accent: "#7EF9FF" },
                                { title: "平均发力", value: backend ? Number(backend.emgMeanLevel).toFixed(1) : "0.0", unit: "lvl", accent: "#73FFCF" },
                                { title: "有效占比", value: backend ? Number(backend.emgEffectiveRatio).toFixed(1) : "0.0", unit: "%", accent: "#44D7B6" },
                                { title: "强发力占比", value: backend ? Number(backend.emgStrongRatio).toFixed(1) : "0.0", unit: "%", accent: "#FF8A3D" }
                            ]

                            delegate: GlassPanel {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                Rectangle {
                                    anchors.fill: parent
                                    radius: parent.radius
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: Qt.rgba(1, 1, 1, 0.015) }
                                        GradientStop { position: 1.0; color: Qt.rgba(1, 1, 1, 0.0) }
                                    }
                                }

                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: 16
                                    spacing: 8

                                    Rectangle {
                                        Layout.preferredWidth: 48
                                        Layout.preferredHeight: 4
                                        radius: 2
                                        color: modelData.accent
                                    }

                                    Text {
                                        text: modelData.title
                                        color: "#DDEBFF"
                                        font.pixelSize: 15
                                    }

                                    Text {
                                        text: "SESSION"
                                        color: "#8FB6D8"
                                        font.pixelSize: 11
                                        font.bold: true
                                        font.letterSpacing: 1.0
                                    }

                                    Row {
                                        spacing: 6

                                        Text {
                                            text: modelData.value
                                            color: "#FAFDFF"
                                            font.pixelSize: 34
                                            font.bold: true
                                        }

                                        Text {
                                            text: modelData.unit
                                            color: "#BBD1EA"
                                            font.pixelSize: 14
                                            anchors.bottom: parent.bottom
                                            anchors.bottomMargin: 7
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
