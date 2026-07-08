/****************************************************************************
** Meta object code from reading C++ file 'HandController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/HandController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HandController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HandController_t {
    QByteArrayData data[58];
    char stringdata0[736];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HandController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HandController_t qt_meta_stringdata_HandController = {
    {
QT_MOC_LITERAL(0, 0, 14), // "HandController"
QT_MOC_LITERAL(1, 15, 21), // "availablePortsChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(4, 62, 15), // "thumbPosChanged"
QT_MOC_LITERAL(5, 78, 15), // "indexPosChanged"
QT_MOC_LITERAL(6, 94, 16), // "middlePosChanged"
QT_MOC_LITERAL(7, 111, 14), // "ringPosChanged"
QT_MOC_LITERAL(8, 126, 15), // "pinkyPosChanged"
QT_MOC_LITERAL(9, 142, 17), // "emgMetricsChanged"
QT_MOC_LITERAL(10, 160, 17), // "emgHistoryChanged"
QT_MOC_LITERAL(11, 178, 12), // "refreshPorts"
QT_MOC_LITERAL(12, 191, 13), // "connectDevice"
QT_MOC_LITERAL(13, 205, 8), // "portName"
QT_MOC_LITERAL(14, 214, 8), // "baudRate"
QT_MOC_LITERAL(15, 223, 16), // "disconnectDevice"
QT_MOC_LITERAL(16, 240, 11), // "setThumbPos"
QT_MOC_LITERAL(17, 252, 3), // "pos"
QT_MOC_LITERAL(18, 256, 11), // "setIndexPos"
QT_MOC_LITERAL(19, 268, 12), // "setMiddlePos"
QT_MOC_LITERAL(20, 281, 10), // "setRingPos"
QT_MOC_LITERAL(21, 292, 11), // "setPinkyPos"
QT_MOC_LITERAL(22, 304, 13), // "sendServoData"
QT_MOC_LITERAL(23, 318, 5), // "thumb"
QT_MOC_LITERAL(24, 324, 5), // "index"
QT_MOC_LITERAL(25, 330, 6), // "middle"
QT_MOC_LITERAL(26, 337, 4), // "ring"
QT_MOC_LITERAL(27, 342, 5), // "pinky"
QT_MOC_LITERAL(28, 348, 16), // "startRehabAction"
QT_MOC_LITERAL(29, 365, 10), // "actionType"
QT_MOC_LITERAL(30, 376, 5), // "speed"
QT_MOC_LITERAL(31, 382, 6), // "cycles"
QT_MOC_LITERAL(32, 389, 13), // "emergencyStop"
QT_MOC_LITERAL(33, 403, 15), // "resetEmgSession"
QT_MOC_LITERAL(34, 419, 15), // "handleReadyRead"
QT_MOC_LITERAL(35, 435, 15), // "handleRehabTick"
QT_MOC_LITERAL(36, 451, 14), // "availablePorts"
QT_MOC_LITERAL(37, 466, 11), // "isConnected"
QT_MOC_LITERAL(38, 478, 17), // "connectionMessage"
QT_MOC_LITERAL(39, 496, 8), // "thumbPos"
QT_MOC_LITERAL(40, 505, 8), // "indexPos"
QT_MOC_LITERAL(41, 514, 9), // "middlePos"
QT_MOC_LITERAL(42, 524, 7), // "ringPos"
QT_MOC_LITERAL(43, 532, 8), // "pinkyPos"
QT_MOC_LITERAL(44, 541, 10), // "emgAverage"
QT_MOC_LITERAL(45, 552, 11), // "emgActivity"
QT_MOC_LITERAL(46, 564, 11), // "emgBaseline"
QT_MOC_LITERAL(47, 576, 8), // "emgDelta"
QT_MOC_LITERAL(48, 585, 8), // "emgLevel"
QT_MOC_LITERAL(49, 594, 12), // "emgPeakLevel"
QT_MOC_LITERAL(50, 607, 12), // "emgMeanLevel"
QT_MOC_LITERAL(51, 620, 12), // "emgIdleRatio"
QT_MOC_LITERAL(52, 633, 17), // "emgEffectiveRatio"
QT_MOC_LITERAL(53, 651, 14), // "emgStrongRatio"
QT_MOC_LITERAL(54, 666, 12), // "emgStateText"
QT_MOC_LITERAL(55, 679, 15), // "emgLevelHistory"
QT_MOC_LITERAL(56, 695, 21), // "emgEffectiveThreshold"
QT_MOC_LITERAL(57, 717, 18) // "emgStrongThreshold"

    },
    "HandController\0availablePortsChanged\0"
    "\0connectionStatusChanged\0thumbPosChanged\0"
    "indexPosChanged\0middlePosChanged\0"
    "ringPosChanged\0pinkyPosChanged\0"
    "emgMetricsChanged\0emgHistoryChanged\0"
    "refreshPorts\0connectDevice\0portName\0"
    "baudRate\0disconnectDevice\0setThumbPos\0"
    "pos\0setIndexPos\0setMiddlePos\0setRingPos\0"
    "setPinkyPos\0sendServoData\0thumb\0index\0"
    "middle\0ring\0pinky\0startRehabAction\0"
    "actionType\0speed\0cycles\0emergencyStop\0"
    "resetEmgSession\0handleReadyRead\0"
    "handleRehabTick\0availablePorts\0"
    "isConnected\0connectionMessage\0thumbPos\0"
    "indexPos\0middlePos\0ringPos\0pinkyPos\0"
    "emgAverage\0emgActivity\0emgBaseline\0"
    "emgDelta\0emgLevel\0emgPeakLevel\0"
    "emgMeanLevel\0emgIdleRatio\0emgEffectiveRatio\0"
    "emgStrongRatio\0emgStateText\0emgLevelHistory\0"
    "emgEffectiveThreshold\0emgStrongThreshold"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HandController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
      22,  182, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  129,    2, 0x06 /* Public */,
       3,    0,  130,    2, 0x06 /* Public */,
       4,    0,  131,    2, 0x06 /* Public */,
       5,    0,  132,    2, 0x06 /* Public */,
       6,    0,  133,    2, 0x06 /* Public */,
       7,    0,  134,    2, 0x06 /* Public */,
       8,    0,  135,    2, 0x06 /* Public */,
       9,    0,  136,    2, 0x06 /* Public */,
      10,    0,  137,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,  138,    2, 0x0a /* Public */,
      12,    2,  139,    2, 0x0a /* Public */,
      15,    0,  144,    2, 0x0a /* Public */,
      16,    1,  145,    2, 0x0a /* Public */,
      18,    1,  148,    2, 0x0a /* Public */,
      19,    1,  151,    2, 0x0a /* Public */,
      20,    1,  154,    2, 0x0a /* Public */,
      21,    1,  157,    2, 0x0a /* Public */,
      22,    5,  160,    2, 0x0a /* Public */,
      28,    3,  171,    2, 0x0a /* Public */,
      32,    0,  178,    2, 0x0a /* Public */,
      33,    0,  179,    2, 0x0a /* Public */,
      34,    0,  180,    2, 0x08 /* Private */,
      35,    0,  181,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   13,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   23,   24,   25,   26,   27,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   29,   30,   31,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      36, QMetaType::QStringList, 0x00495001,
      37, QMetaType::Bool, 0x00495001,
      38, QMetaType::QString, 0x00495001,
      39, QMetaType::Int, 0x00495103,
      40, QMetaType::Int, 0x00495103,
      41, QMetaType::Int, 0x00495103,
      42, QMetaType::Int, 0x00495103,
      43, QMetaType::Int, 0x00495103,
      44, QMetaType::Int, 0x00495001,
      45, QMetaType::Int, 0x00495001,
      46, QMetaType::Int, 0x00495001,
      47, QMetaType::Int, 0x00495001,
      48, QMetaType::Int, 0x00495001,
      49, QMetaType::Int, 0x00495001,
      50, QMetaType::Double, 0x00495001,
      51, QMetaType::Double, 0x00495001,
      52, QMetaType::Double, 0x00495001,
      53, QMetaType::Double, 0x00495001,
      54, QMetaType::QString, 0x00495001,
      55, QMetaType::QVariantList, 0x00495001,
      56, QMetaType::Int, 0x00095401,
      57, QMetaType::Int, 0x00095401,

 // properties: notify_signal_id
       0,
       1,
       1,
       2,
       3,
       4,
       5,
       6,
       7,
       7,
       7,
       7,
       7,
       7,
       7,
       7,
       7,
       7,
       7,
       8,
       0,
       0,

       0        // eod
};

void HandController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HandController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->availablePortsChanged(); break;
        case 1: _t->connectionStatusChanged(); break;
        case 2: _t->thumbPosChanged(); break;
        case 3: _t->indexPosChanged(); break;
        case 4: _t->middlePosChanged(); break;
        case 5: _t->ringPosChanged(); break;
        case 6: _t->pinkyPosChanged(); break;
        case 7: _t->emgMetricsChanged(); break;
        case 8: _t->emgHistoryChanged(); break;
        case 9: _t->refreshPorts(); break;
        case 10: _t->connectDevice((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->disconnectDevice(); break;
        case 12: _t->setThumbPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->setIndexPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->setMiddlePos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->setRingPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->setPinkyPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->sendServoData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 18: _t->startRehabAction((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 19: _t->emergencyStop(); break;
        case 20: _t->resetEmgSession(); break;
        case 21: _t->handleReadyRead(); break;
        case 22: _t->handleRehabTick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::availablePortsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::connectionStatusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::thumbPosChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::indexPosChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::middlePosChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::ringPosChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::pinkyPosChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::emgMetricsChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (HandController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HandController::emgHistoryChanged)) {
                *result = 8;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<HandController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isConnected(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->connectionMessage(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->thumbPos(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->indexPos(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->middlePos(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->ringPos(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->pinkyPos(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->emgAverage(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->emgActivity(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->emgBaseline(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->emgDelta(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->emgLevel(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->emgPeakLevel(); break;
        case 14: *reinterpret_cast< double*>(_v) = _t->emgMeanLevel(); break;
        case 15: *reinterpret_cast< double*>(_v) = _t->emgIdleRatio(); break;
        case 16: *reinterpret_cast< double*>(_v) = _t->emgEffectiveRatio(); break;
        case 17: *reinterpret_cast< double*>(_v) = _t->emgStrongRatio(); break;
        case 18: *reinterpret_cast< QString*>(_v) = _t->emgStateText(); break;
        case 19: *reinterpret_cast< QVariantList*>(_v) = _t->emgLevelHistory(); break;
        case 20: *reinterpret_cast< int*>(_v) = _t->emgEffectiveThreshold(); break;
        case 21: *reinterpret_cast< int*>(_v) = _t->emgStrongThreshold(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<HandController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 3: _t->setThumbPos(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setIndexPos(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setMiddlePos(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setRingPos(*reinterpret_cast< int*>(_v)); break;
        case 7: _t->setPinkyPos(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject HandController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_HandController.data,
    qt_meta_data_HandController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HandController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HandController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HandController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HandController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 23;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 22;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 22;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 22;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 22;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 22;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void HandController::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HandController::connectionStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HandController::thumbPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void HandController::indexPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void HandController::middlePosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void HandController::ringPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void HandController::pinkyPosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void HandController::emgMetricsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void HandController::emgHistoryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
