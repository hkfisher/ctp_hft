/****************************************************************************
** Meta object code from reading C++ file 'Trade.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/Trade.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Trade.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_future__Trade_t {
    QByteArrayData data[13];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_future__Trade_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_future__Trade_t qt_meta_stringdata_future__Trade = {
    {
QT_MOC_LITERAL(0, 0, 13), // "future::Trade"
QT_MOC_LITERAL(1, 14, 17), // "signals_write_log"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 3), // "str"
QT_MOC_LITERAL(4, 37, 21), // "signals_state_changed"
QT_MOC_LITERAL(5, 59, 5), // "state"
QT_MOC_LITERAL(6, 65, 6), // "effect"
QT_MOC_LITERAL(7, 72, 10), // "deal_price"
QT_MOC_LITERAL(8, 83, 22), // "signals_close_position"
QT_MOC_LITERAL(9, 106, 12), // "commodity_no"
QT_MOC_LITERAL(10, 119, 11), // "contract_no"
QT_MOC_LITERAL(11, 131, 22), // "signals_withdraw_order"
QT_MOC_LITERAL(12, 154, 8) // "order_no"

    },
    "future::Trade\0signals_write_log\0\0str\0"
    "signals_state_changed\0state\0effect\0"
    "deal_price\0signals_close_position\0"
    "commodity_no\0contract_no\0"
    "signals_withdraw_order\0order_no"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_future__Trade[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    3,   37,    2, 0x06 /* Public */,
       8,    2,   44,    2, 0x06 /* Public */,
      11,    1,   49,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Char, QMetaType::Char, QMetaType::QString,    5,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   10,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

void future::Trade::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Trade *_t = static_cast<Trade *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signals_write_log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signals_state_changed((*reinterpret_cast< char(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->signals_close_position((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->signals_withdraw_order((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Trade::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Trade::signals_write_log)) {
                *result = 0;
            }
        }
        {
            typedef void (Trade::*_t)(char , char , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Trade::signals_state_changed)) {
                *result = 1;
            }
        }
        {
            typedef void (Trade::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Trade::signals_close_position)) {
                *result = 2;
            }
        }
        {
            typedef void (Trade::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Trade::signals_withdraw_order)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject future::Trade::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_future__Trade.data,
      qt_meta_data_future__Trade,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *future::Trade::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *future::Trade::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_future__Trade.stringdata0))
        return static_cast<void*>(const_cast< Trade*>(this));
    if (!strcmp(_clname, "ITapTradeAPINotify"))
        return static_cast< ITapTradeAPINotify*>(const_cast< Trade*>(this));
    return QObject::qt_metacast(_clname);
}

int future::Trade::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void future::Trade::signals_write_log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void future::Trade::signals_state_changed(char _t1, char _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void future::Trade::signals_close_position(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void future::Trade::signals_withdraw_order(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
