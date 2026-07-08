/****************************************************************************
** Meta object code from reading C++ file 'HandController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/HandController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HandController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14HandControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto HandController::qt_create_metaobjectdata<qt_meta_tag_ZN14HandControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "HandController",
        "availablePortsChanged",
        "",
        "connectionStatusChanged",
        "thumbPosChanged",
        "indexPosChanged",
        "middlePosChanged",
        "ringPosChanged",
        "pinkyPosChanged",
        "emgMetricsChanged",
        "emgHistoryChanged",
        "refreshPorts",
        "connectDevice",
        "portName",
        "baudRate",
        "disconnectDevice",
        "setThumbPos",
        "pos",
        "setIndexPos",
        "setMiddlePos",
        "setRingPos",
        "setPinkyPos",
        "sendServoData",
        "thumb",
        "index",
        "middle",
        "ring",
        "pinky",
        "startRehabAction",
        "actionType",
        "speed",
        "cycles",
        "emergencyStop",
        "resetEmgSession",
        "handleReadyRead",
        "handleRehabTick",
        "availablePorts",
        "isConnected",
        "thumbPos",
        "indexPos",
        "middlePos",
        "ringPos",
        "pinkyPos",
        "emgAverage",
        "emgActivity",
        "emgBaseline",
        "emgDelta",
        "emgLevel",
        "emgPeakLevel",
        "emgMeanLevel",
        "emgIdleRatio",
        "emgEffectiveRatio",
        "emgStrongRatio",
        "emgStateText",
        "emgLevelHistory",
        "QVariantList",
        "emgEffectiveThreshold",
        "emgStrongThreshold"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'availablePortsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'connectionStatusChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'thumbPosChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'indexPosChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'middlePosChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'ringPosChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pinkyPosChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'emgMetricsChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'emgHistoryChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'refreshPorts'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'connectDevice'
        QtMocHelpers::SlotData<void(const QString &, int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 }, { QMetaType::Int, 14 },
        }}),
        // Slot 'disconnectDevice'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setThumbPos'
        QtMocHelpers::SlotData<void(int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Slot 'setIndexPos'
        QtMocHelpers::SlotData<void(int)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Slot 'setMiddlePos'
        QtMocHelpers::SlotData<void(int)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Slot 'setRingPos'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Slot 'setPinkyPos'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Slot 'sendServoData'
        QtMocHelpers::SlotData<void(int, int, int, int, int)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 }, { QMetaType::Int, 25 }, { QMetaType::Int, 26 },
            { QMetaType::Int, 27 },
        }}),
        // Slot 'startRehabAction'
        QtMocHelpers::SlotData<void(int, int, int)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 29 }, { QMetaType::Int, 30 }, { QMetaType::Int, 31 },
        }}),
        // Slot 'emergencyStop'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'resetEmgSession'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'handleReadyRead'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleRehabTick'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'availablePorts'
        QtMocHelpers::PropertyData<QStringList>(36, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'isConnected'
        QtMocHelpers::PropertyData<bool>(37, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'thumbPos'
        QtMocHelpers::PropertyData<int>(38, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'indexPos'
        QtMocHelpers::PropertyData<int>(39, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'middlePos'
        QtMocHelpers::PropertyData<int>(40, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'ringPos'
        QtMocHelpers::PropertyData<int>(41, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'pinkyPos'
        QtMocHelpers::PropertyData<int>(42, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
        // property 'emgAverage'
        QtMocHelpers::PropertyData<int>(43, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'emgActivity'
        QtMocHelpers::PropertyData<int>(44, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'emgBaseline'
        QtMocHelpers::PropertyData<int>(45, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'emgDelta'
        QtMocHelpers::PropertyData<int>(46, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'emgLevel'
        QtMocHelpers::PropertyData<int>(47, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'emgPeakLevel'
        QtMocHelpers::PropertyData<int>(48, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'emgMeanLevel'
        QtMocHelpers::PropertyData<double>(49, QMetaType::Double, QMC::DefaultPropertyFlags, 7),
        // property 'emgIdleRatio'
        QtMocHelpers::PropertyData<double>(50, QMetaType::Double, QMC::DefaultPropertyFlags, 7),
        // property 'emgEffectiveRatio'
        QtMocHelpers::PropertyData<double>(51, QMetaType::Double, QMC::DefaultPropertyFlags, 7),
        // property 'emgStrongRatio'
        QtMocHelpers::PropertyData<double>(52, QMetaType::Double, QMC::DefaultPropertyFlags, 7),
        // property 'emgStateText'
        QtMocHelpers::PropertyData<QString>(53, QMetaType::QString, QMC::DefaultPropertyFlags, 7),
        // property 'emgLevelHistory'
        QtMocHelpers::PropertyData<QVariantList>(54, 0x80000000 | 55, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 8),
        // property 'emgEffectiveThreshold'
        QtMocHelpers::PropertyData<int>(56, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'emgStrongThreshold'
        QtMocHelpers::PropertyData<int>(57, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<HandController, qt_meta_tag_ZN14HandControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject HandController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14HandControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14HandControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14HandControllerE_t>.metaTypes,
    nullptr
} };

void HandController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HandController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
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
        case 10: _t->connectDevice((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 11: _t->disconnectDevice(); break;
        case 12: _t->setThumbPos((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->setIndexPos((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->setMiddlePos((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->setRingPos((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->setPinkyPos((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->sendServoData((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5]))); break;
        case 18: _t->startRehabAction((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 19: _t->emergencyStop(); break;
        case 20: _t->resetEmgSession(); break;
        case 21: _t->handleReadyRead(); break;
        case 22: _t->handleRehabTick(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::availablePortsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::connectionStatusChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::thumbPosChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::indexPosChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::middlePosChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::ringPosChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::pinkyPosChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::emgMetricsChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (HandController::*)()>(_a, &HandController::emgHistoryChanged, 8))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QStringList*>(_v) = _t->availablePorts(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isConnected(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->thumbPos(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->indexPos(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->middlePos(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->ringPos(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->pinkyPos(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->emgAverage(); break;
        case 8: *reinterpret_cast<int*>(_v) = _t->emgActivity(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->emgBaseline(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->emgDelta(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->emgLevel(); break;
        case 12: *reinterpret_cast<int*>(_v) = _t->emgPeakLevel(); break;
        case 13: *reinterpret_cast<double*>(_v) = _t->emgMeanLevel(); break;
        case 14: *reinterpret_cast<double*>(_v) = _t->emgIdleRatio(); break;
        case 15: *reinterpret_cast<double*>(_v) = _t->emgEffectiveRatio(); break;
        case 16: *reinterpret_cast<double*>(_v) = _t->emgStrongRatio(); break;
        case 17: *reinterpret_cast<QString*>(_v) = _t->emgStateText(); break;
        case 18: *reinterpret_cast<QVariantList*>(_v) = _t->emgLevelHistory(); break;
        case 19: *reinterpret_cast<int*>(_v) = _t->emgEffectiveThreshold(); break;
        case 20: *reinterpret_cast<int*>(_v) = _t->emgStrongThreshold(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setThumbPos(*reinterpret_cast<int*>(_v)); break;
        case 3: _t->setIndexPos(*reinterpret_cast<int*>(_v)); break;
        case 4: _t->setMiddlePos(*reinterpret_cast<int*>(_v)); break;
        case 5: _t->setRingPos(*reinterpret_cast<int*>(_v)); break;
        case 6: _t->setPinkyPos(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *HandController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HandController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14HandControllerE_t>.strings))
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
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 23;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
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
