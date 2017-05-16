/****************************************************************************
** Meta object code from reading C++ file 'qttrajectoryanalysis.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qttrajectoryanalysis.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qttrajectoryanalysis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtTrajectoryAnalysis[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      44,   21,   21,   21, 0x08,
      67,   21,   21,   21, 0x08,
      98,   21,   21,   21, 0x08,
     122,   21,   21,   21, 0x08,
     145,   21,   21,   21, 0x08,
     167,   21,   21,   21, 0x08,
     182,   21,   21,   21, 0x08,
     198,   21,   21,   21, 0x08,
     214,   21,   21,   21, 0x08,
     234,   21,   21,   21, 0x08,
     263,   21,   21,   21, 0x08,
     278,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtTrajectoryAnalysis[] = {
    "QtTrajectoryAnalysis\0\0on_pBtn_Run_clicked()\0"
    "on_pBtn_Stop_clicked()\0"
    "on_pBtn_OutputFolder_clicked()\0"
    "on_pBtn_Check_clicked()\0on_pBtn_quit_clicked()\0"
    "updateProcessBar(int)\0getThreadEnd()\0"
    "getThreadStop()\0getThreadFail()\0"
    "updateImg(cv::Mat&)\0updateEventInfo(std::string)\0"
    "closeWaitDlg()\0batchDone()\0"
};

void QtTrajectoryAnalysis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtTrajectoryAnalysis *_t = static_cast<QtTrajectoryAnalysis *>(_o);
        switch (_id) {
        case 0: _t->on_pBtn_Run_clicked(); break;
        case 1: _t->on_pBtn_Stop_clicked(); break;
        case 2: _t->on_pBtn_OutputFolder_clicked(); break;
        case 3: _t->on_pBtn_Check_clicked(); break;
        case 4: _t->on_pBtn_quit_clicked(); break;
        case 5: _t->updateProcessBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->getThreadEnd(); break;
        case 7: _t->getThreadStop(); break;
        case 8: _t->getThreadFail(); break;
        case 9: _t->updateImg((*reinterpret_cast< cv::Mat(*)>(_a[1]))); break;
        case 10: _t->updateEventInfo((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 11: _t->closeWaitDlg(); break;
        case 12: _t->batchDone(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtTrajectoryAnalysis::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtTrajectoryAnalysis::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QtTrajectoryAnalysis,
      qt_meta_data_QtTrajectoryAnalysis, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtTrajectoryAnalysis::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtTrajectoryAnalysis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtTrajectoryAnalysis::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtTrajectoryAnalysis))
        return static_cast<void*>(const_cast< QtTrajectoryAnalysis*>(this));
    return QDialog::qt_metacast(_clname);
}

int QtTrajectoryAnalysis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
