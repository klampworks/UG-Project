/****************************************************************************
** Meta object code from reading C++ file 'gui.h'
**
** Created: Sun Oct 20 14:23:56 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_gui[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      34,    4,    4,    4, 0x08,
      58,    4,    4,    4, 0x08,
      81,    4,    4,    4, 0x08,
     105,    4,    4,    4, 0x08,
     131,    4,    4,    4, 0x08,
     160,    4,    4,    4, 0x08,
     182,    4,    4,    4, 0x08,
     205,    4,    4,    4, 0x08,
     228,    4,    4,    4, 0x08,
     248,    4,    4,    4, 0x08,
     268,    4,    4,    4, 0x08,
     292,    4,    4,    4, 0x08,
     316,    4,    4,    4, 0x08,
     340,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_gui[] = {
    "gui\0\0file_select_button_clicked()\0"
    "lookup_button_pressed()\0solve_button_pressed()\0"
    "thread_follow_changed()\0"
    "thread_selected_changed()\0"
    "solution_selection_changed()\0"
    "play_button_pressed()\0forward_word_pressed()\0"
    "forward_clue_pressed()\0dict_conf_changed()\0"
    "thes_conf_changed()\0web_wiki_conf_changed()\0"
    "web_goog_conf_changed()\0web_clue_conf_changed()\0"
    "keep_going_button_pressed()\0"
};

void gui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        gui *_t = static_cast<gui *>(_o);
        switch (_id) {
        case 0: _t->file_select_button_clicked(); break;
        case 1: _t->lookup_button_pressed(); break;
        case 2: _t->solve_button_pressed(); break;
        case 3: _t->thread_follow_changed(); break;
        case 4: _t->thread_selected_changed(); break;
        case 5: _t->solution_selection_changed(); break;
        case 6: _t->play_button_pressed(); break;
        case 7: _t->forward_word_pressed(); break;
        case 8: _t->forward_clue_pressed(); break;
        case 9: _t->dict_conf_changed(); break;
        case 10: _t->thes_conf_changed(); break;
        case 11: _t->web_wiki_conf_changed(); break;
        case 12: _t->web_goog_conf_changed(); break;
        case 13: _t->web_clue_conf_changed(); break;
        case 14: _t->keep_going_button_pressed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData gui::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject gui::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_gui,
      qt_meta_data_gui, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &gui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *gui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *gui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_gui))
        return static_cast<void*>(const_cast< gui*>(this));
    return QWidget::qt_metacast(_clname);
}

int gui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
