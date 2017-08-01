/****************************************************************************
** Meta object code from reading C++ file 'Quote.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/Quote.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Quote.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_future__Quote_t {
    QByteArrayData data[7];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_future__Quote_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_future__Quote_t qt_meta_stringdata_future__Quote = {
    {
QT_MOC_LITERAL(0, 0, 13), // "future::Quote"
QT_MOC_LITERAL(1, 14, 17), // "signals_write_log"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 3), // "str"
QT_MOC_LITERAL(4, 37, 21), // "signals_quote_changed"
QT_MOC_LITERAL(5, 59, 10), // "last_price"
QT_MOC_LITERAL(6, 70, 23) // "signals_quote_reconnect"

    },
    "future::Quote\0signals_write_log\0\0str\0"
    "signals_quote_changed\0last_price\0"
    "signals_quote_reconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_future__Quote[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,
       6,    0,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

       0        // eod
};

void future::Quote::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Quote *_t = static_cast<Quote *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signals_write_log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signals_quote_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->signals_quote_reconnect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Quote::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Quote::signals_write_log)) {
                *result = 0;
            }
        }
        {
            typedef void (Quote::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Quote::signals_quote_changed)) {
                *result = 1;
            }
        }
        {
            typedef void (Quote::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Quote::signals_quote_reconnect)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject future::Quote::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_future__Quote.data,
      qt_meta_data_future__Quote,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *future::Quote::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *future::Quote::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_future__Quote.stringdata0))
        return static_cast<void*>(const_cast< Quote*>(this));
    if (!strcmp(_clname, "CThostFtdcMdSpi"))
        return static_cast< CThostFtdcMdSpi*>(const_cast< Quote*>(this));
    return QObject::qt_metacast(_clname);
}

int future::Quote::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void future::Quote::signals_write_log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void future::Quote::signals_quote_changed(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void future::Quote::signals_quote_reconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
