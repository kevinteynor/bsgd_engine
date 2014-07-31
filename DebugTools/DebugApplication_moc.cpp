/****************************************************************************
** Meta object code from reading C++ file 'DebugApplication.h'
**
** Created: Tue Mar 18 17:44:09 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DebugApplication.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DebugApplication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DebugTools__DebugApplication[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   29,   29,   29, 0x0a,
      54,   29,   29,   29, 0x0a,
      76,   29,   29,   29, 0x0a,
      98,   29,   29,   29, 0x0a,
     112,  107,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DebugTools__DebugApplication[] = {
    "DebugTools::DebugApplication\0\0"
    "MousePressed(glm::vec2)\0MouseMoved(glm::vec2)\0"
    "RendererInitialized()\0Update()\0area\0"
    "DockWidgetOrientationChanged(Qt::DockWidgetArea)\0"
};

void DebugTools::DebugApplication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DebugApplication *_t = static_cast<DebugApplication *>(_o);
        switch (_id) {
        case 0: _t->MousePressed((*reinterpret_cast< glm::vec2(*)>(_a[1]))); break;
        case 1: _t->MouseMoved((*reinterpret_cast< glm::vec2(*)>(_a[1]))); break;
        case 2: _t->RendererInitialized(); break;
        case 3: _t->Update(); break;
        case 4: _t->DockWidgetOrientationChanged((*reinterpret_cast< Qt::DockWidgetArea(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DebugTools::DebugApplication::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DebugTools::DebugApplication::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DebugTools__DebugApplication,
      qt_meta_data_DebugTools__DebugApplication, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DebugTools::DebugApplication::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DebugTools::DebugApplication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DebugTools::DebugApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DebugTools__DebugApplication))
        return static_cast<void*>(const_cast< DebugApplication*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DebugTools::DebugApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_DebugTools__DebugWidget[] = {

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

static const char qt_meta_stringdata_DebugTools__DebugWidget[] = {
    "DebugTools::DebugWidget\0"
};

void DebugTools::DebugWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DebugTools::DebugWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DebugTools::DebugWidget::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DebugTools__DebugWidget,
      qt_meta_data_DebugTools__DebugWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DebugTools::DebugWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DebugTools::DebugWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DebugTools::DebugWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DebugTools__DebugWidget))
        return static_cast<void*>(const_cast< DebugWidget*>(this));
    return QObject::qt_metacast(_clname);
}

int DebugTools::DebugWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DebugTools__ButtonWatchInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   28,   28,   28, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DebugTools__ButtonWatchInfo[] = {
    "DebugTools::ButtonWatchInfo\0\0"
    "functioncall()\0"
};

void DebugTools::ButtonWatchInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ButtonWatchInfo *_t = static_cast<ButtonWatchInfo *>(_o);
        switch (_id) {
        case 0: _t->functioncall(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DebugTools::ButtonWatchInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DebugTools::ButtonWatchInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DebugTools__ButtonWatchInfo,
      qt_meta_data_DebugTools__ButtonWatchInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DebugTools::ButtonWatchInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DebugTools::ButtonWatchInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DebugTools::ButtonWatchInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DebugTools__ButtonWatchInfo))
        return static_cast<void*>(const_cast< ButtonWatchInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int DebugTools::ButtonWatchInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
