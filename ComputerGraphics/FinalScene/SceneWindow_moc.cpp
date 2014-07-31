/****************************************************************************
** Meta object code from reading C++ file 'SceneWindow.h'
**
** Created: Tue Mar 18 17:44:16 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SceneWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SceneWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SceneWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_SceneWindow[] = {
    "SceneWindow\0"
};

void SceneWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SceneWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SceneWindow::staticMetaObject = {
    { &DebugApplication::staticMetaObject, qt_meta_stringdata_SceneWindow,
      qt_meta_data_SceneWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SceneWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SceneWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SceneWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SceneWindow))
        return static_cast<void*>(const_cast< SceneWindow*>(this));
    return DebugApplication::qt_metacast(_clname);
}

int SceneWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DebugApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
