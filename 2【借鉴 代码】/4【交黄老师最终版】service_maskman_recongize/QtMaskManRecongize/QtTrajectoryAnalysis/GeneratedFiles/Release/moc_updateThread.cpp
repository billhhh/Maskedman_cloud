/****************************************************************************
** Meta object code from reading C++ file 'updateThread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../updateThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'updateThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_updateThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      33,   13,   13,   13, 0x05,
      51,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      73,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_updateThread[] = {
    "updateThread\0\0sendSliderVal(int)\0"
    "sendImg(cv::Mat*)\0sendInfo(std::string)\0"
    "getThreadEnd()\0"
};

void updateThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        updateThread *_t = static_cast<updateThread *>(_o);
        switch (_id) {
        case 0: _t->sendSliderVal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sendImg((*reinterpret_cast< cv::Mat*(*)>(_a[1]))); break;
        case 2: _t->sendInfo((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 3: _t->getThreadEnd(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData updateThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject updateThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_updateThread,
      qt_meta_data_updateThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &updateThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *updateThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *updateThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_updateThread))
        return static_cast<void*>(const_cast< updateThread*>(this));
    return QThread::qt_metacast(_clname);
}

int updateThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void updateThread::sendSliderVal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void updateThread::sendImg(cv::Mat * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void updateThread::sendInfo(std::string _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
