/****************************************************************************
** Meta object code from reading C++ file 'box.h'
**
** Created: Tue Jul 12 22:56:40 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../box.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'box.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BoxGroup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      25,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,    9,    9,    9, 0x0a,
      66,   57,    9,    9, 0x0a,
      82,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BoxGroup[] = {
    "BoxGroup\0\0need_new_box()\0game_finished()\0"
    "move_one_step()\0interval\0startTimer(int)\0"
    "stop_timer()\0"
};

void BoxGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BoxGroup *_t = static_cast<BoxGroup *>(_o);
        switch (_id) {
        case 0: _t->need_new_box(); break;
        case 1: _t->game_finished(); break;
        case 2: _t->move_one_step(); break;
        case 3: _t->startTimer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->stop_timer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BoxGroup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BoxGroup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BoxGroup,
      qt_meta_data_BoxGroup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BoxGroup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BoxGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BoxGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BoxGroup))
        return static_cast<void*>(const_cast< BoxGroup*>(this));
    if (!strcmp(_clname, "QGraphicsItemGroup"))
        return static_cast< QGraphicsItemGroup*>(const_cast< BoxGroup*>(this));
    return QObject::qt_metacast(_clname);
}

int BoxGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void BoxGroup::need_new_box()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void BoxGroup::game_finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
