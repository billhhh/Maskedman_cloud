/****************************************************************************
** Meta object code from reading C++ file 'extractThread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../extractThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'extractThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_extractThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      34,   14,   14,   14, 0x05,
      47,   14,   14,   14, 0x05,
      59,   14,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_extractThread[] = {
    "extractThread\0\0sendSliderVal(int)\0"
    "threadStop()\0threadEnd()\0threadFail()\0"
};

void extractThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        extractThread *_t = static_cast<extractThread *>(_o);
        switch (_id) {
        case 0: _t->sendSliderVal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->threadStop(); break;
        case 2: _t->threadEnd(); break;
        case 3: _t->threadFail(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData extractThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject extractThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_extractThread,
      qt_meta_data_extractThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &extractThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *extractThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *extractThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_extractThread))
        return static_cast<void*>(const_cast< extractThread*>(this));
    return QThread::qt_metacast(_clname);
}

int extractThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void extractThread::sendSliderVal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void extractThread::threadStop()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void extractThread::threadEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void extractThread::threadFail()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
