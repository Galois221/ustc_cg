/****************************************************************************
** Meta object code from reading C++ file 'ImageWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ImageWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImageWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ImageWidget_t {
    QByteArrayData data[20];
    char stringdata0[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageWidget_t qt_meta_stringdata_ImageWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ImageWidget"
QT_MOC_LITERAL(1, 12, 4), // "Open"
QT_MOC_LITERAL(2, 17, 7), // "QImage*"
QT_MOC_LITERAL(3, 25, 0), // ""
QT_MOC_LITERAL(4, 26, 4), // "Save"
QT_MOC_LITERAL(5, 31, 6), // "SaveAs"
QT_MOC_LITERAL(6, 38, 6), // "Invert"
QT_MOC_LITERAL(7, 45, 6), // "Mirror"
QT_MOC_LITERAL(8, 52, 10), // "horizontal"
QT_MOC_LITERAL(9, 63, 8), // "vertical"
QT_MOC_LITERAL(10, 72, 8), // "TurnGray"
QT_MOC_LITERAL(11, 81, 7), // "Restore"
QT_MOC_LITERAL(12, 89, 12), // "StartSetting"
QT_MOC_LITERAL(13, 102, 12), // "ClearSetting"
QT_MOC_LITERAL(14, 115, 21), // "change_points_visible"
QT_MOC_LITERAL(15, 137, 10), // "IDWWarping"
QT_MOC_LITERAL(16, 148, 6), // "setgap"
QT_MOC_LITERAL(17, 155, 10), // "RBFWarping"
QT_MOC_LITERAL(18, 166, 8), // "setratio"
QT_MOC_LITERAL(19, 175, 5) // "value"

    },
    "ImageWidget\0Open\0QImage*\0\0Save\0SaveAs\0"
    "Invert\0Mirror\0horizontal\0vertical\0"
    "TurnGray\0Restore\0StartSetting\0"
    "ClearSetting\0change_points_visible\0"
    "IDWWarping\0setgap\0RBFWarping\0setratio\0"
    "value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    3, 0x0a /* Public */,
       4,    0,  100,    3, 0x0a /* Public */,
       5,    0,  101,    3, 0x0a /* Public */,
       6,    0,  102,    3, 0x0a /* Public */,
       7,    2,  103,    3, 0x0a /* Public */,
       7,    1,  108,    3, 0x2a /* Public | MethodCloned */,
       7,    0,  111,    3, 0x2a /* Public | MethodCloned */,
      10,    0,  112,    3, 0x0a /* Public */,
      11,    0,  113,    3, 0x0a /* Public */,
      12,    0,  114,    3, 0x0a /* Public */,
      13,    0,  115,    3, 0x0a /* Public */,
      14,    0,  116,    3, 0x0a /* Public */,
      15,    1,  117,    3, 0x0a /* Public */,
      15,    0,  120,    3, 0x2a /* Public | MethodCloned */,
      17,    1,  121,    3, 0x0a /* Public */,
      17,    0,  124,    3, 0x2a /* Public | MethodCloned */,
      18,    1,  125,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    8,    9,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,

       0        // eod
};

void ImageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QImage* _r = _t->Open();
            if (_a[0]) *reinterpret_cast< QImage**>(_a[0]) = std::move(_r); }  break;
        case 1: _t->Save(); break;
        case 2: _t->SaveAs(); break;
        case 3: _t->Invert(); break;
        case 4: _t->Mirror((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->Mirror((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->Mirror(); break;
        case 7: _t->TurnGray(); break;
        case 8: _t->Restore(); break;
        case 9: _t->StartSetting(); break;
        case 10: _t->ClearSetting(); break;
        case 11: { bool _r = _t->change_points_visible();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->IDWWarping((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->IDWWarping(); break;
        case 14: _t->RBFWarping((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->RBFWarping(); break;
        case 16: _t->setratio((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ImageWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ImageWidget.data,
    qt_meta_data_ImageWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ImageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
