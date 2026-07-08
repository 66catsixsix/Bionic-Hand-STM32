#ifndef HANDCONTROLLER_H
#define HANDCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QVariantList>

class HandController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged)
    Q_PROPERTY(QString connectionMessage READ connectionMessage NOTIFY connectionStatusChanged)
    
    // 各个手指的位置，范围0~100 (百分比)
    Q_PROPERTY(int thumbPos READ thumbPos WRITE setThumbPos NOTIFY thumbPosChanged)
    Q_PROPERTY(int indexPos READ indexPos WRITE setIndexPos NOTIFY indexPosChanged)
    Q_PROPERTY(int middlePos READ middlePos WRITE setMiddlePos NOTIFY middlePosChanged)
    Q_PROPERTY(int ringPos READ ringPos WRITE setRingPos NOTIFY ringPosChanged)
    Q_PROPERTY(int pinkyPos READ pinkyPos WRITE setPinkyPos NOTIFY pinkyPosChanged)
    Q_PROPERTY(int emgAverage READ emgAverage NOTIFY emgMetricsChanged)
    Q_PROPERTY(int emgActivity READ emgActivity NOTIFY emgMetricsChanged)
    Q_PROPERTY(int emgBaseline READ emgBaseline NOTIFY emgMetricsChanged)
    Q_PROPERTY(int emgDelta READ emgDelta NOTIFY emgMetricsChanged)
    Q_PROPERTY(int emgLevel READ emgLevel NOTIFY emgMetricsChanged)
    Q_PROPERTY(int emgPeakLevel READ emgPeakLevel NOTIFY emgMetricsChanged)
    Q_PROPERTY(double emgMeanLevel READ emgMeanLevel NOTIFY emgMetricsChanged)
    Q_PROPERTY(double emgIdleRatio READ emgIdleRatio NOTIFY emgMetricsChanged)
    Q_PROPERTY(double emgEffectiveRatio READ emgEffectiveRatio NOTIFY emgMetricsChanged)
    Q_PROPERTY(double emgStrongRatio READ emgStrongRatio NOTIFY emgMetricsChanged)
    Q_PROPERTY(QString emgStateText READ emgStateText NOTIFY emgMetricsChanged)
    Q_PROPERTY(QVariantList emgLevelHistory READ emgLevelHistory NOTIFY emgHistoryChanged)
    Q_PROPERTY(int emgEffectiveThreshold READ emgEffectiveThreshold CONSTANT)
    Q_PROPERTY(int emgStrongThreshold READ emgStrongThreshold CONSTANT)

public:
    explicit HandController(QObject *parent = nullptr);
    ~HandController();

    QStringList availablePorts() const;
    bool isConnected() const;
    QString connectionMessage() const;

    int thumbPos() const;
    int indexPos() const;
    int middlePos() const;
    int ringPos() const;
    int pinkyPos() const;
    int emgAverage() const;
    int emgActivity() const;
    int emgBaseline() const;
    int emgDelta() const;
    int emgLevel() const;
    int emgPeakLevel() const;
    double emgMeanLevel() const;
    double emgIdleRatio() const;
    double emgEffectiveRatio() const;
    double emgStrongRatio() const;
    QString emgStateText() const;
    QVariantList emgLevelHistory() const;
    int emgEffectiveThreshold() const;
    int emgStrongThreshold() const;

public slots:
    void refreshPorts();
    void connectDevice(const QString &portName, int baudRate);
    void disconnectDevice();

    void setThumbPos(int pos);
    void setIndexPos(int pos);
    void setMiddlePos(int pos);
    void setRingPos(int pos);
    void setPinkyPos(int pos);
    
    // QML主动发送一帧数据（整体控制所有舵机）
    void sendServoData(int thumb, int index, int middle, int ring, int pinky);
    
    // 康复模式的动作序列
    void startRehabAction(int actionType, int speed, int cycles);
    void emergencyStop();
    void resetEmgSession();

signals:
    void availablePortsChanged();
    void connectionStatusChanged();
    void thumbPosChanged();
    void indexPosChanged();
    void middlePosChanged();
    void ringPosChanged();
    void pinkyPosChanged();
    void emgMetricsChanged();
    void emgHistoryChanged();

private slots:
    void handleReadyRead();
    void handleRehabTick();

private:
    int angleToPercent(int angleDeg) const;
    void updateFingers(int t, int i, int m, int r, int p);
    void parseFingerDebugLine(const QString &line);
    void parseEmgLine(const QString &line);
    void appendEmgLevel(int level);

    QSerialPort *m_serial;
    QStringList m_ports;
    QByteArray m_rxBuffer;
    QString m_connectionMessage;
    
    int m_thumb, m_index, m_middle, m_ring, m_pinky;
    int m_emgAverage;
    int m_emgActivity;
    int m_emgBaseline;
    int m_emgDelta;
    int m_emgLevel;
    int m_emgPeakLevel;
    quint64 m_emgLevelSum;
    quint64 m_emgTotalSamples;
    quint64 m_emgIdleSamples;
    quint64 m_emgEffectiveSamples;
    quint64 m_emgStrongSamples;
    QVariantList m_emgLevelHistory;
    static constexpr int kEmgHistoryLimit = 180;
    static constexpr int kEmgEffectiveThreshold = 30;
    static constexpr int kEmgStrongThreshold = 80;
    
    // 模拟康复动画时的定时器
    QTimer *m_rehabTimer;
    int m_rehabCycleCounter;
    int m_rehabStep;
    int m_rehabActionType;
};

#endif // HANDCONTROLLER_H
