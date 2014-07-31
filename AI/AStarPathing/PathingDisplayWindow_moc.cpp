/****************************************************************************
** Meta object code from reading C++ file 'PathingDisplayWindow.h'
**
** Created: Thu Oct 17 13:50:21 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PathingDisplayWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PathingDisplayWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PathingDisplayWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x0a,
      31,   21,   21,   21, 0x0a,
      54,   21,   21,   21, 0x0a,
      72,   66,   21,   21, 0x0a,
     105,   99,   21,   21, 0x0a,
     133,   99,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PathingDisplayWindow[] = {
    "PathingDisplayWindow\0\0Update()\0"
    "InitializeComponents()\0OpenLevel()\0"
    "value\0UpdateFollowerSpeed(float)\0state\0"
    "SetPathNodesVisibility(int)\0"
    "SetCurrentPathVisibility(int)\0"
};

void PathingDisplayWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PathingDisplayWindow *_t = static_cast<PathingDisplayWindow *>(_o);
        switch (_id) {
        case 0: _t->Update(); break;
        case 1: _t->InitializeComponents(); break;
        case 2: _t->OpenLevel(); break;
        case 3: _t->UpdateFollowerSpeed((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->SetPathNodesVisibility((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->SetCurrentPathVisibility((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PathingDisplayWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PathingDisplayWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PathingDisplayWindow,
      qt_meta_data_PathingDisplayWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PathingDisplayWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PathingDisplayWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PathingDisplayWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PathingDisplayWindow))
        return static_cast<void*>(const_cast< PathingDisplayWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PathingDisplayWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
