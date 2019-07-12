/****************************************************************************
** Meta object code from reading C++ file 'xmlsetting.h'
**
** Created: Fri Apr 26 09:01:22 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../xmlsetting.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'xmlsetting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_XmlSetting[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   12,   11,   11, 0x08,
      75,   12,   11,   11, 0x08,
     119,   12,   11,   11, 0x08,
     163,   12,   11,   11, 0x08,
     207,   11,   11,   11, 0x08,
     237,   11,   11,   11, 0x08,
     266,   11,   11,   11, 0x08,
     294,   11,   11,   11, 0x08,
     319,   11,   11,   11, 0x08,
     344,   11,   11,   11, 0x08,
     369,   11,   11,   11, 0x08,
     391,   11,   11,   11, 0x08,
     415,   11,   11,   11, 0x08,
     435,   11,   11,   11, 0x08,
     457,   11,   11,   11, 0x08,
     479,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_XmlSetting[] = {
    "XmlSetting\0\0pItem,column\0"
    "on_m_tree_itemDoubleClicked(QTreeWidgetItem*,int)\0"
    "on_m_tree_itemChanged(QTreeWidgetItem*,int)\0"
    "on_m_tree_itemEntered(QTreeWidgetItem*,int)\0"
    "on_m_tree_itemPressed(QTreeWidgetItem*,int)\0"
    "m_buildNewFileAct_triggered()\0"
    "m_openNewFileAct_triggered()\0"
    "m_saveAsFileAct_triggered()\0"
    "m_addNodeAct_triggered()\0"
    "m_addAttrAct_triggered()\0"
    "m_addComment_triggered()\0m_addText_triggered()\0"
    "m_deleteAct_triggered()\0m_upAct_triggered()\0"
    "m_downAct_triggered()\0m_copyAct_triggered()\0"
    "m_pasteAct_triggered()\0"
};

const QMetaObject XmlSetting::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_XmlSetting,
      qt_meta_data_XmlSetting, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &XmlSetting::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *XmlSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *XmlSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_XmlSetting))
        return static_cast<void*>(const_cast< XmlSetting*>(this));
    return QWidget::qt_metacast(_clname);
}

int XmlSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_m_tree_itemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: on_m_tree_itemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: on_m_tree_itemEntered((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: on_m_tree_itemPressed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: m_buildNewFileAct_triggered(); break;
        case 5: m_openNewFileAct_triggered(); break;
        case 6: m_saveAsFileAct_triggered(); break;
        case 7: m_addNodeAct_triggered(); break;
        case 8: m_addAttrAct_triggered(); break;
        case 9: m_addComment_triggered(); break;
        case 10: m_addText_triggered(); break;
        case 11: m_deleteAct_triggered(); break;
        case 12: m_upAct_triggered(); break;
        case 13: m_downAct_triggered(); break;
        case 14: m_copyAct_triggered(); break;
        case 15: m_pasteAct_triggered(); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
