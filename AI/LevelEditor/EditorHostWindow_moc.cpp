/****************************************************************************
** Meta object code from reading C++ file 'EditorHostWindow.h'
**
** Created: Wed Oct 30 09:42:40 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditorHostWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EditorHostWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EditorHostWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      27,   17,   17,   17, 0x0a,
      50,   17,   17,   17, 0x0a,
      68,   17,   17,   17, 0x0a,
      83,   17,   17,   17, 0x0a,
     106,   17,   17,   17, 0x0a,
     117,   17,   17,   17, 0x0a,
     134,   17,   17,   17, 0x0a,
     154,   17,   17,   17, 0x0a,
     179,   17,   17,   17, 0x0a,
     212,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EditorHostWindow[] = {
    "EditorHostWindow\0\0Update()\0"
    "InitializeComponents()\0OpenFile_Native()\0"
    "OpenFile_FBX()\0OpenFile_NativeLevel()\0"
    "SaveFile()\0AddPathingNode()\0"
    "RemoveUnusedNodes()\0SetPathingNodePosition()\0"
    "Renderer_MousePressed(glm::vec2)\0"
    "Renderer_MouseMoved(glm::vec2)\0"
};

void EditorHostWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EditorHostWindow *_t = static_cast<EditorHostWindow *>(_o);
        switch (_id) {
        case 0: _t->Update(); break;
        case 1: _t->InitializeComponents(); break;
        case 2: _t->OpenFile_Native(); break;
        case 3: _t->OpenFile_FBX(); break;
        case 4: _t->OpenFile_NativeLevel(); break;
        case 5: _t->SaveFile(); break;
        case 6: _t->AddPathingNode(); break;
        case 7: _t->RemoveUnusedNodes(); break;
        case 8: _t->SetPathingNodePosition(); break;
        case 9: _t->Renderer_MousePressed((*reinterpret_cast< glm::vec2(*)>(_a[1]))); break;
        case 10: _t->Renderer_MouseMoved((*reinterpret_cast< glm::vec2(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EditorHostWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EditorHostWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_EditorHostWindow,
      qt_meta_data_EditorHostWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EditorHostWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EditorHostWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EditorHostWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EditorHostWindow))
        return static_cast<void*>(const_cast< EditorHostWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int EditorHostWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
