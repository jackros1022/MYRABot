/****************************************************************************
** Meta object code from reading C++ file 'ObjectsDetectedCallback.h'
**
** Created: Fri Apr 4 18:04:18 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ObjectsDetectedCallback.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ObjectsDetectedCallback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ObjectsDetectedCallback[] = {

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
      41,   25,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ObjectsDetectedCallback[] = {
    "ObjectsDetectedCallback\0\0objectsDetected\0"
    "publish(QMap<int,QPair<QRect,QTransform> >)\0"
};

void ObjectsDetectedCallback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ObjectsDetectedCallback *_t = static_cast<ObjectsDetectedCallback *>(_o);
        switch (_id) {
        case 0: _t->publish((*reinterpret_cast< const QMap<int,QPair<QRect,QTransform> >(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ObjectsDetectedCallback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ObjectsDetectedCallback::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ObjectsDetectedCallback,
      qt_meta_data_ObjectsDetectedCallback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ObjectsDetectedCallback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ObjectsDetectedCallback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ObjectsDetectedCallback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ObjectsDetectedCallback))
        return static_cast<void*>(const_cast< ObjectsDetectedCallback*>(this));
    return QObject::qt_metacast(_clname);
}

int ObjectsDetectedCallback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
