/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata0[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 4), // "Open"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 4), // "Save"
QT_MOC_LITERAL(4, 22, 6), // "SaveAs"
QT_MOC_LITERAL(5, 29, 17), // "CreateChildWindow"
QT_MOC_LITERAL(6, 47, 12), // "ChildWindow*"
QT_MOC_LITERAL(7, 60, 18), // "SetActiveSubWindow"
QT_MOC_LITERAL(8, 79, 8), // "QWidget*"
QT_MOC_LITERAL(9, 88, 6), // "window"
QT_MOC_LITERAL(10, 95, 6), // "Invert"
QT_MOC_LITERAL(11, 102, 6), // "Mirror"
QT_MOC_LITERAL(12, 109, 9), // "GrayScale"
QT_MOC_LITERAL(13, 119, 7), // "Restore"
QT_MOC_LITERAL(14, 127, 10), // "ChooseRect"
QT_MOC_LITERAL(15, 138, 10), // "ChoosePoly"
QT_MOC_LITERAL(16, 149, 14), // "ChooseFreehand"
QT_MOC_LITERAL(17, 164, 5), // "Paste"
QT_MOC_LITERAL(18, 170, 9), // "Paste_mix"
QT_MOC_LITERAL(19, 180, 4) // "Copy"

    },
    "MainWindow\0Open\0\0Save\0SaveAs\0"
    "CreateChildWindow\0ChildWindow*\0"
    "SetActiveSubWindow\0QWidget*\0window\0"
    "Invert\0Mirror\0GrayScale\0Restore\0"
    "ChooseRect\0ChoosePoly\0ChooseFreehand\0"
    "Paste\0Paste_mix\0Copy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       7,    1,   93,    2, 0x08 /* Private */,
      10,    0,   96,    2, 0x08 /* Private */,
      11,    0,   97,    2, 0x08 /* Private */,
      12,    0,   98,    2, 0x08 /* Private */,
      13,    0,   99,    2, 0x08 /* Private */,
      14,    0,  100,    2, 0x08 /* Private */,
      15,    0,  101,    2, 0x08 /* Private */,
      16,    0,  102,    2, 0x08 /* Private */,
      17,    0,  103,    2, 0x08 /* Private */,
      18,    0,  104,    2, 0x08 /* Private */,
      19,    0,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Open(); break;
        case 1: _t->Save(); break;
        case 2: _t->SaveAs(); break;
        case 3: { ChildWindow* _r = _t->CreateChildWindow();
            if (_a[0]) *reinterpret_cast< ChildWindow**>(_a[0]) = std::move(_r); }  break;
        case 4: _t->SetActiveSubWindow((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 5: _t->Invert(); break;
        case 6: _t->Mirror(); break;
        case 7: _t->GrayScale(); break;
        case 8: _t->Restore(); break;
        case 9: _t->ChooseRect(); break;
        case 10: _t->ChoosePoly(); break;
        case 11: _t->ChooseFreehand(); break;
        case 12: _t->Paste(); break;
        case 13: _t->Paste_mix(); break;
        case 14: _t->Copy(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
