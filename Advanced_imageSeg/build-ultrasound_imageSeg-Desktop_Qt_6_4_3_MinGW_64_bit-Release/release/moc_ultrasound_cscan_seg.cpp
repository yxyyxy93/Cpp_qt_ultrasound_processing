/****************************************************************************
** Meta object code from reading C++ file 'ultrasound_cscan_seg.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ultrasound_imageSeg/ultrasound_cscan_seg.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ultrasound_cscan_seg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ultrasound_cscan_seg_t {
    uint offsetsAndSizes[20];
    char stringdata0[21];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[12];
    char stringdata5[5];
    char stringdata6[10];
    char stringdata7[22];
    char stringdata8[13];
    char stringdata9[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ultrasound_cscan_seg_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ultrasound_cscan_seg_t qt_meta_stringdata_ultrasound_cscan_seg = {
    {
        QT_MOC_LITERAL(0, 20),  // "ultrasound_cscan_seg"
        QT_MOC_LITERAL(21, 18),  // "handleButton_Cscan"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 17),  // "handleButton_draw"
        QT_MOC_LITERAL(59, 11),  // "onLineDrawn"
        QT_MOC_LITERAL(71, 4),  // "line"
        QT_MOC_LITERAL(76, 9),  // "saveImage"
        QT_MOC_LITERAL(86, 21),  // "handleButton_multiSNR"
        QT_MOC_LITERAL(108, 12),  // "selectFolder"
        QT_MOC_LITERAL(121, 14)   // "closeDrawnArea"
    },
    "ultrasound_cscan_seg",
    "handleButton_Cscan",
    "",
    "handleButton_draw",
    "onLineDrawn",
    "line",
    "saveImage",
    "handleButton_multiSNR",
    "selectFolder",
    "closeDrawnArea"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ultrasound_cscan_seg[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x08,    1 /* Private */,
       3,    0,   57,    2, 0x08,    2 /* Private */,
       4,    1,   58,    2, 0x08,    3 /* Private */,
       6,    0,   61,    2, 0x08,    5 /* Private */,
       7,    0,   62,    2, 0x08,    6 /* Private */,
       8,    0,   63,    2, 0x08,    7 /* Private */,
       9,    0,   64,    2, 0x0a,    8 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QLineF,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ultrasound_cscan_seg::staticMetaObject = { {
    QMetaObject::SuperData::link<ultrasound_Cscan_process::staticMetaObject>(),
    qt_meta_stringdata_ultrasound_cscan_seg.offsetsAndSizes,
    qt_meta_data_ultrasound_cscan_seg,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ultrasound_cscan_seg_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ultrasound_cscan_seg, std::true_type>,
        // method 'handleButton_Cscan'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleButton_draw'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLineDrawn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QLineF &, std::false_type>,
        // method 'saveImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleButton_multiSNR'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectFolder'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'closeDrawnArea'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ultrasound_cscan_seg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ultrasound_cscan_seg *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->handleButton_Cscan(); break;
        case 1: _t->handleButton_draw(); break;
        case 2: _t->onLineDrawn((*reinterpret_cast< std::add_pointer_t<QLineF>>(_a[1]))); break;
        case 3: _t->saveImage(); break;
        case 4: _t->handleButton_multiSNR(); break;
        case 5: _t->selectFolder(); break;
        case 6: _t->closeDrawnArea(); break;
        default: ;
        }
    }
}

const QMetaObject *ultrasound_cscan_seg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ultrasound_cscan_seg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ultrasound_cscan_seg.stringdata0))
        return static_cast<void*>(this);
    return ultrasound_Cscan_process::qt_metacast(_clname);
}

int ultrasound_cscan_seg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ultrasound_Cscan_process::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
