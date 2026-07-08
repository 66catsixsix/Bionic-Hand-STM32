#include "HandController.h"
#include <QDebug>
#include <QRegularExpression>
#include <QThread>

HandController::HandController(QObject *parent)
    : QObject(parent), m_serial(new QSerialPort(this)),
      m_thumb(50), m_index(50), m_middle(50), m_ring(50), m_pinky(50),
      m_emgAverage(0), m_emgActivity(0), m_emgBaseline(0), m_emgDelta(0), m_emgLevel(0),
      m_emgPeakLevel(0), m_emgLevelSum(0), m_emgTotalSamples(0), m_emgIdleSamples(0),
      m_emgEffectiveSamples(0), m_emgStrongSamples(0),
      m_connectionMessage(QStringLiteral("请选择串口并建立连接")),
      m_rehabCycleCounter(0), m_rehabStep(0), m_rehabActionType(0)
{
    m_rehabTimer = new QTimer(this);
    connect(m_rehabTimer, &QTimer::timeout, this, &HandController::handleRehabTick);
    connect(m_serial, &QSerialPort::readyRead, this, &HandController::handleReadyRead);
    connect(m_serial, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        if(error == QSerialPort::NoError) {
            return;
        }
        m_connectionMessage = QStringLiteral("串口错误：%1").arg(m_serial->errorString());
        emit connectionStatusChanged();
    });
    
    refreshPorts();
}

HandController::~HandController()
{
    if (m_serial->isOpen())
        m_serial->close();
}

QStringList HandController::availablePorts() const
{
    return m_ports;
}

void HandController::refreshPorts()
{
    m_ports.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        m_ports << info.portName();
    }
    emit availablePortsChanged();
}

bool HandController::isConnected() const
{
    return m_serial->isOpen();
}

QString HandController::connectionMessage() const
{
    return m_connectionMessage;
}

void HandController::connectDevice(const QString &portName, int baudRate)
{
    if (m_serial->isOpen()) {
        m_serial->close();
    }
    
    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    
    if (m_serial->open(QIODevice::ReadWrite)) {
        m_rxBuffer.clear();
        m_connectionMessage = QStringLiteral("已连接 %1 @ %2").arg(portName).arg(baudRate);
        qDebug() << "Connected to" << portName << "@" << baudRate;
    } else {
        m_connectionMessage = QStringLiteral("连接失败：%1").arg(m_serial->errorString());
        qDebug() << "Failed to connect to" << portName;
    }
    emit connectionStatusChanged();
}

void HandController::disconnectDevice()
{
    if (m_serial->isOpen()) {
        m_serial->close();
        m_rxBuffer.clear();
        m_connectionMessage = QStringLiteral("串口已断开");
        qDebug() << "Disconnected device";
    }
    emit connectionStatusChanged();
}

void HandController::sendServoData(int thumb, int index, int middle, int ring, int pinky)
{
    thumb = qBound(0, thumb, 100);
    index = qBound(0, index, 100);
    middle = qBound(0, middle, 100);
    ring = qBound(0, ring, 100);
    pinky = qBound(0, pinky, 100);

    setThumbPos(thumb);
    setIndexPos(index);
    setMiddlePos(middle);
    setRingPos(ring);
    setPinkyPos(pinky);
}

void HandController::handleReadyRead()
{
    m_rxBuffer += m_serial->readAll();

    int newlineIndex = -1;
    while((newlineIndex = m_rxBuffer.indexOf('\n')) >= 0) {
        const QByteArray lineBytes = m_rxBuffer.left(newlineIndex).trimmed();
        m_rxBuffer.remove(0, newlineIndex + 1);

        if(lineBytes.isEmpty()) {
            continue;
        }

        const QString line = QString::fromUtf8(lineBytes);
        parseFingerDebugLine(line);
        parseEmgLine(line);
    }
}

int HandController::angleToPercent(int angleDeg) const
{
    constexpr int kOpenAngle = 49;
    constexpr int kCloseAngle = 190;
    const int span = kCloseAngle - kOpenAngle;

    if(span <= 0) {
        return 0;
    }

    return qBound(0, ((angleDeg - kOpenAngle) * 100) / span, 100);
}

void HandController::updateFingers(int t, int i, int m, int r, int p)
{
    // 统一设置并发送信号，用来触发UI的动画效果
    setThumbPos(t);
    setIndexPos(i);
    setMiddlePos(m);
    setRingPos(r);
    setPinkyPos(p);
}

void HandController::parseFingerDebugLine(const QString &line)
{
    static const QRegularExpression fdbgRegex(
        QStringLiteral(R"(FDBG idx=(\d+)\s+id=(\d+)\s+ch=(\d+)\s+cal=(\d+)\s+raw=(\d+)\s+med=(\d+)\s+pos=(\d+)\s+ang=(-?\d+)\s+spd=(\d+)\s+d=(\d+))"));

    const QRegularExpressionMatch match = fdbgRegex.match(line);
    int idx;
    int percent;

    if(!match.hasMatch()) {
        return;
    }

    idx = match.captured(1).toInt();
    percent = angleToPercent(match.captured(8).toInt());

    switch(idx) {
    case 0:
        setThumbPos(percent);
        break;
    case 1:
        setIndexPos(percent);
        break;
    case 2:
        setMiddlePos(percent);
        break;
    case 3:
        setRingPos(percent);
        break;
    case 4:
        setPinkyPos(percent);
        break;
    default:
        break;
    }
}

// ============== 手指位置的Getter和Setter ============== //
int HandController::thumbPos() const { return m_thumb; }
void HandController::setThumbPos(int pos) {
    if (m_thumb != pos) {
        m_thumb = pos;
        emit thumbPosChanged();
		// 同步到串口，下同
		// sendServoData(m_thumb, m_index, m_middle, m_ring, m_pinky);
    }
}

int HandController::indexPos() const { return m_index; }
void HandController::setIndexPos(int pos) {
    if (m_index != pos) {
        m_index = pos;
        emit indexPosChanged();
    }
}

int HandController::middlePos() const { return m_middle; }
void HandController::setMiddlePos(int pos) {
    if (m_middle != pos) {
        m_middle = pos;
        emit middlePosChanged();
    }
}

int HandController::ringPos() const { return m_ring; }
void HandController::setRingPos(int pos) {
    if (m_ring != pos) {
        m_ring = pos;
        emit ringPosChanged();
    }
}

int HandController::pinkyPos() const { return m_pinky; }
void HandController::setPinkyPos(int pos) {
    if (m_pinky != pos) {
        m_pinky = pos;
        emit pinkyPosChanged();
    }
}

int HandController::emgAverage() const { return m_emgAverage; }
int HandController::emgActivity() const { return m_emgActivity; }
int HandController::emgBaseline() const { return m_emgBaseline; }
int HandController::emgDelta() const { return m_emgDelta; }
int HandController::emgLevel() const { return m_emgLevel; }
int HandController::emgPeakLevel() const { return m_emgPeakLevel; }

double HandController::emgMeanLevel() const
{
    if(m_emgTotalSamples == 0U) {
        return 0.0;
    }
    return static_cast<double>(m_emgLevelSum) / static_cast<double>(m_emgTotalSamples);
}

double HandController::emgIdleRatio() const
{
    if(m_emgTotalSamples == 0U) {
        return 0.0;
    }
    return static_cast<double>(m_emgIdleSamples) * 100.0 / static_cast<double>(m_emgTotalSamples);
}

double HandController::emgEffectiveRatio() const
{
    if(m_emgTotalSamples == 0U) {
        return 0.0;
    }
    return static_cast<double>(m_emgEffectiveSamples) * 100.0 / static_cast<double>(m_emgTotalSamples);
}

double HandController::emgStrongRatio() const
{
    if(m_emgTotalSamples == 0U) {
        return 0.0;
    }
    return static_cast<double>(m_emgStrongSamples) * 100.0 / static_cast<double>(m_emgTotalSamples);
}

QString HandController::emgStateText() const
{
    if(m_emgLevel >= kEmgStrongThreshold) {
        return QStringLiteral("强发力");
    }
    if(m_emgLevel >= kEmgEffectiveThreshold) {
        return QStringLiteral("有效发力");
    }
    return QStringLiteral("无发力");
}

QVariantList HandController::emgLevelHistory() const
{
    return m_emgLevelHistory;
}

int HandController::emgEffectiveThreshold() const { return kEmgEffectiveThreshold; }
int HandController::emgStrongThreshold() const { return kEmgStrongThreshold; }

void HandController::resetEmgSession()
{
    m_emgAverage = 0;
    m_emgActivity = 0;
    m_emgBaseline = 0;
    m_emgDelta = 0;
    m_emgLevel = 0;
    m_emgPeakLevel = 0;
    m_emgLevelSum = 0;
    m_emgTotalSamples = 0;
    m_emgIdleSamples = 0;
    m_emgEffectiveSamples = 0;
    m_emgStrongSamples = 0;
    m_emgLevelHistory.clear();
    emit emgHistoryChanged();
    emit emgMetricsChanged();
}

// ============== 康复控制模式 ============== //
void HandController::startRehabAction(int actionType, int speed, int cycles)
{
    m_rehabActionType = actionType;
    m_rehabCycleCounter = cycles;
    m_rehabStep = 0;
    
    int intervalMs = 1000 / speed; // 这里的速度调节到定时器时间
    m_rehabTimer->start(intervalMs); // 开启定时器
    qDebug() << "Started Rehab mode, type:" << actionType << "Speed:" << intervalMs << "Cycles:" << cycles;
}

void HandController::emergencyStop()
{
    m_rehabTimer->stop();
    qDebug() << "EMERGENCY STOP";
    // 如果需要可以给下位机发一个急停指令
    sendServoData(m_thumb, m_index, m_middle, m_ring, m_pinky); 
}

void HandController::handleRehabTick()
{
    if (m_rehabCycleCounter <= 0) {
        m_rehabTimer->stop();
        return;
    }
    
    // 这里生成一个模拟的手部波浪、握拳等动作流。由于我们在QML中加了Behavior，此处只需要将目标数值写成0或者100，UI自然会产生非常丝滑平缓的过渡动画！
    if (m_rehabActionType == 0) { // 握拳
        if (m_rehabStep % 2 == 0) {
            updateFingers(90, 100, 100, 100, 100);
            sendServoData(90, 100, 100, 100, 100);
        } else {
            updateFingers(0, 0, 0, 0, 0);
            sendServoData(0, 0, 0, 0, 0);
            m_rehabCycleCounter--;
        }
    } else if (m_rehabActionType == 1) { // 波浪动作
        // 依次伸展手指
        switch (m_rehabStep % 6) {
            case 0: setThumbPos(100); break;
            case 1: setIndexPos(100); break;
            case 2: setMiddlePos(100); break;
            case 3: setRingPos(100); break;
            case 4: setPinkyPos(100); break;
            case 5: updateFingers(0, 0, 0, 0, 0); m_rehabCycleCounter--; break;
        }
        sendServoData(m_thumb, m_index, m_middle, m_ring, m_pinky);
    }
    m_rehabStep++;
}

void HandController::parseEmgLine(const QString &line)
{
    static const QRegularExpression emgRegex(
        QStringLiteral(R"(EMG avg=(\d+)\s+act=(\d+)\s+base=(\d+)\s+delta=(-?\d+)\s+lvl=(\d+))"));

    const QRegularExpressionMatch match = emgRegex.match(line);
    if(!match.hasMatch()) {
        return;
    }

    m_emgAverage = match.captured(1).toInt();
    m_emgActivity = match.captured(2).toInt();
    m_emgBaseline = match.captured(3).toInt();
    m_emgDelta = qMax(0, match.captured(4).toInt());
    m_emgLevel = qBound(0, match.captured(5).toInt(), 100);

    if(m_emgLevel > m_emgPeakLevel) {
        m_emgPeakLevel = m_emgLevel;
    }

    m_emgTotalSamples++;
    m_emgLevelSum += static_cast<quint64>(m_emgLevel);

    if(m_emgLevel >= kEmgStrongThreshold) {
        m_emgStrongSamples++;
    } else if(m_emgLevel >= kEmgEffectiveThreshold) {
        m_emgEffectiveSamples++;
    } else {
        m_emgIdleSamples++;
    }

    appendEmgLevel(m_emgLevel);
    emit emgMetricsChanged();
}

void HandController::appendEmgLevel(int level)
{
    m_emgLevelHistory.append(level);
    while(m_emgLevelHistory.size() > kEmgHistoryLimit) {
        m_emgLevelHistory.removeFirst();
    }
    emit emgHistoryChanged();
}
