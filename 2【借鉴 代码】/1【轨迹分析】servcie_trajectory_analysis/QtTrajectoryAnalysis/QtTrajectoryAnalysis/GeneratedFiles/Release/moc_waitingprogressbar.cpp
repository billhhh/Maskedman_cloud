/****************************************************************************
** Meta object code from reading C++ file 'waitingprogressbar.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../waitingprogressbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'waitingprogressbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WaitingProgressBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WaitingProgressBar[] = {
    "WaitingProgressBar\0\0procDone()\0"
    "on_cmdLinkBtn_clicked()\0"
};

void WaitingProgressBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WaitingProgressBar *_t = static_cast<WaitingProgressBar *>(_o);
        switch (_id) {
        case 0: _t->procDone(); break;
        case 1: _t->on_cmdLinkBtn_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData WaitingProgressBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WaitingProgressBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WaitingProgressBar,
      qt_meta_data_WaitingProgressBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WaitingProgressBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WaitingProgressBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WaitingProgressBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WaitingProgressBar))
        return static_cast<void*>(const_cast< WaitingProgressBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int WaitingProgressBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void WaitingProgressBar::procDone()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
