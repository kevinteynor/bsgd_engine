/****************************************************************************
** Meta object code from reading C++ file 'CaptureTheFlag.h'
**
** Created: Tue Sep 17 04:27:50 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CaptureTheFlag.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CaptureTheFlag.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CaptureTheFlag[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      25,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CaptureTheFlag[] = {
    "CaptureTheFlag\0\0Update()\0OpenLevel()\0"
};

void CaptureTheFlag::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CaptureTheFlag *_t = static_cast<CaptureTheFlag *>(_o);
        switch (_id) {
        case 0: _t->Update(); break;
        case 1: _t->OpenLevel(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CaptureTheFlag::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CaptureTheFlag::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CaptureTheFlag,
      qt_meta_data_CaptureTheFlag, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CaptureTheFlag::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CaptureTheFlag::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CaptureTheFlag::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CaptureTheFlag))
        return static_cast<void*>(const_cast< CaptureTheFlag*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CaptureTheFlag::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
