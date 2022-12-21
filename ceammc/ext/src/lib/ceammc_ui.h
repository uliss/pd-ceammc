#ifndef CEAMMC_NEW_GUI_H
#define CEAMMC_NEW_GUI_H

#include "ceammc.h"
#include "ceammc_atomlist.h"
#include "ceammc_externals.h"
#include "ceammc_log.h"
#include "ceammc_ui_object.h"
#include "ceammc_object_info.h"
#include "ceammc_syms.h"

#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace ceammc {

class UIObject;
class UIDspObject;

constexpr const char* DEFAULT_ACTIVE_COLOR = "0. 0.75 1. 1.";
constexpr const char* DEFAULT_BORDER_COLOR = "0.6 0.6 0.6 1.";
constexpr const char* DEFAULT_BACKGROUND_COLOR = "0.93 0.93 0.93 1.";
constexpr const char* DEFAULT_TEXT_COLOR = "0. 0. 0. 1.";
constexpr const char* DEFAULT_LABEL_COLOR = "0. 0. 0. 1.";

constexpr const char* PROP_ACTIVE_COLOR = "active_color";
constexpr const char* PROP_BACKGROUND_COLOR = "background_color";
constexpr const char* PROP_BORDER_COLOR = "border_color";
constexpr const char* PROP_TEXT_COLOR = "text_color";
constexpr const char* PROP_LABEL_COLOR = "label_color";

constexpr const char* PROP_PRESET_NAME = "presetname";

//! Gettext extract message helper
#ifndef _
#define _(msg) msg
#endif

#define UI_METHOD_PTR(m) reinterpret_cast<t_typ_method>(&UIObjectFactory<UI>::m)

// clang-format off
enum MouseEvents {
    UI_MOUSE_DOWN      = 1,
    UI_MOUSE_UP        = 1 << 1,
    UI_MOUSE_DRAG      = 1 << 2,
    UI_MOUSE_ENTER     = 1 << 3,
    UI_MOUSE_LEAVE     = 1 << 4,
    UI_MOUSE_MOVE      = 1 << 5,
    UI_MOUSE_DBL_CLICK = 1 << 6,
    UI_MOUSE_WHEEL     = 1 << 7
};
// clang-format on

enum class MouseEventsOutput {
    DEFAULT_OFF,
    DEFAULT_ON
};

template <class UI>
class UIObjectFactory {
    std::string name_;

public:
    using isControlObject = std::is_base_of<UIObject, UI>;
    using isDSPObject = std::is_base_of<UIDspObject, UI>;

    using bangMethodPtr = void (UI::*)();
    using floatMethodPtr = void (UI::*)(t_float);
    using listMethodPtr = void (UI::*)(const AtomListView&);

    using BangMethodMap = std::unordered_map<t_symbol*, bangMethodPtr>;
    using FloatMethodMap = std::unordered_map<t_symbol*, floatMethodPtr>;
    using ListMethodMap = std::unordered_map<t_symbol*, listMethodPtr>;

    using propAtomGet = Atom (UI::*)() const;
    using propAtomSet = void (UI::*)(const Atom&);
    using propBoolGet = bool (UI::*)() const;
    using propBoolSet = void (UI::*)(bool);
    using propIntGet = t_int (UI::*)() const;
    using propIntSet = void (UI::*)(t_int);
    using propFloatGet = t_float (UI::*)() const;
    using propFloatSet = void (UI::*)(t_float);
    using propSymbolGet = t_symbol* (UI::*)() const;
    using propSymbolSet = void (UI::*)(t_symbol*);
    using propListGet = AtomList (UI::*)() const;
    using propListSet = void (UI::*)(const AtomListView&);

    using propertyAtomAccess = std::pair<propAtomGet, propAtomSet>;
    using propertyBoolAccess = std::pair<propBoolGet, propBoolSet>;
    using propertyIntAccess = std::pair<propIntGet, propIntSet>;
    using propertyFloatAccess = std::pair<propFloatGet, propFloatSet>;
    using propertySymbolAccess = std::pair<propSymbolGet, propSymbolSet>;
    using propertyListAccess = std::pair<propListGet, propListSet>;

    using AtomPropertyMap = std::unordered_map<t_symbol*, propertyAtomAccess>;
    using BoolPropertyMap = std::unordered_map<t_symbol*, propertyBoolAccess>;
    using IntPropertyMap = std::unordered_map<t_symbol*, propertyIntAccess>;
    using FloatPropertyMap = std::unordered_map<t_symbol*, propertyFloatAccess>;
    using SymbolPropertyMap = std::unordered_map<t_symbol*, propertySymbolAccess>;
    using ListPropertyMap = std::unordered_map<t_symbol*, propertyListAccess>;

public:
    UIObjectFactory(const char* name, long fl = EBOX_GROWINDI, int pd_flags = 0)
        : name_(name)
    {
        flags = fl;
        if (pd_ui_class) {
            pd_error(0, "class already initialized: '%s'", name);
            return;
        }

        pd_ui_class = eclass_new(name_.c_str(),
            UI_METHOD_PTR(alloc),
            UI_METHOD_PTR(free),
            sizeof(UI), pd_flags, A_GIMME, 0);

        if (!pd_ui_class) {
            pd_error(0, "can't allocate class: '%s'", name);
            return;
        }

        cicmInit();
        setupMethods();

        setupAttributes();
        register_ui_external(&pd_ui_class->c_class);
    }

    ~UIObjectFactory()
    {
        if (pd_ui_class) {
            eclass_attr_sort(pd_ui_class);
            eclass_register(CLASS_BOX, pd_ui_class);
        }
    }

    static UI* alloc(t_symbol* s, int argc, t_atom* argv)
    {
        void* mem = eobj_new(pd_ui_class);
        t_binbuf* d = binbuf_via_atoms(argc, argv);

        if (mem && d) {
            UI* x = new (mem) UI();

            ebox_new(x->asEBox(), 0 | flags);
            dspInit(x);
            ebox_attrprocess_viabinbuf(x->asEBox(), d);
            ebox_ready(x->asEBox());
            binbuf_free(d);

            using InitMethodType = void (UI::*)(t_symbol*, const AtomListView&, bool);
            InitMethodType minit = &UI::init;

            x->init(s, AtomListView(argv, argc), use_presets);
            return x;
        }

        return 0;
    }

    static void free(UI* x)
    {
        if (x) {
            x->~UI();
            ebox_free(x->asEBox());
        }
    }

    template <class X = void>
    typename std::enable_if<isDSPObject::value, X>::type cicmInit()
    {
        eclass_dspinit(pd_ui_class);
        eclass_guiinit(pd_ui_class, flags);
    }

    template <class X = void>
    typename std::enable_if<isControlObject::value, X>::type cicmInit()
    {
        eclass_guiinit(pd_ui_class, flags);
    }

    template <class X = void>
    static typename std::enable_if<isDSPObject::value, X>::type dspInit(UI* x)
    {
        x->dspInit();
    }

    template <class X = void>
    static typename std::enable_if<isControlObject::value, X>::type dspInit(UI*)
    {
    }

    template <typename X = void>
    typename std::enable_if<isDSPObject::value, X>::type setupMethods()
    {
        // clang-format off
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(dsp),           str_dsp,           A_NULL,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(paint),         str_paint,         A_GIMME, 0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(notify),        str_notify,        A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(okSize),        str_oksize,        A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(onZoom),        str_onzoom,        A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(setDrawParams), str_getdrawparams, A_NULL, 0);
        // clang-format on
    }

    template <typename X = void>
    typename std::enable_if<isControlObject::value, X>::type setupMethods()
    {
        // clang-format off
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(paint),         str_paint,         A_GIMME, 0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(notify),        str_notify,        A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(okSize),        str_oksize,        A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(onZoom),        str_onzoom,        A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(setDrawParams), str_getdrawparams, A_NULL, 0);
        // clang-format on
    }

    static void dsp(UI* z, t_object* dsp,
        short* count, double samplerate, long maxvectorsize, long flags)
    {
        z->dspOn(samplerate, maxvectorsize);
        object_method(dsp, sym_dsp_add(), z, UI_METHOD_PTR(dspProcess), 0, NULL);
    }

    static void dspProcess(UI* z, t_object* dsp,
        t_sample** ins, long n_ins,
        t_sample** outs, long n_outs,
        long sampleframes, long flags, void* up)
    {
        z->dspProcess(ins, n_ins, outs, n_outs, sampleframes);
    }

    void setupAttributes()
    {
        hideProperty("fontweight");
        hideProperty("fontslant");

        // clang-format off
        // background / border color
        addProperty(PROP_BACKGROUND_COLOR,
                    _("Background Color"),
                    DEFAULT_BACKGROUND_COLOR,
                    &UI::prop_color_background);

        addProperty(PROP_BORDER_COLOR,
                    _("Border Color"),
                    DEFAULT_BORDER_COLOR,
                    &UI::prop_color_border);

        addProperty(PROP_LABEL_COLOR,
                    _("Label Color"),
                    DEFAULT_LABEL_COLOR,
                    &UI::prop_color_label);

        // default
        CLASS_ATTR_DEFAULT              (pd_ui_class, str_size, "45. 15.");
        // clang-format on
    }

    void usePresets()
    {
        // clang-format off

        UIObjectFactory::use_presets = true;

        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(loadPreset),      "load",          A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(interpPreset),    "interp",        A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(storePreset),     "store",         A_GIMME,  0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(clearPreset),     "clear",         A_GIMME,  0);

        CLASS_ATTR_SYMBOL   (pd_ui_class, PROP_PRESET_NAME, t_ebox, b_objpreset_id);
        CLASS_ATTR_DEFAULT  (pd_ui_class, PROP_PRESET_NAME,  "(null)");
        CLASS_ATTR_SAVE     (pd_ui_class, PROP_PRESET_NAME);
        CLASS_ATTR_CATEGORY (pd_ui_class, PROP_PRESET_NAME, _("Basic"));
        CLASS_ATTR_LABEL    (pd_ui_class, PROP_PRESET_NAME, _("Preset Name"));
        CLASS_ATTR_ACCESSORS(pd_ui_class, PROP_PRESET_NAME, nullptr, ebox_set_presetid);

        // clang-format on
    }

    void usePopup()
    {
        // clang-format off
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(showPopup),      str_rightclick,    A_GIMME, 0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(processPopup),   str_popup,         A_GIMME, 0);
        // clang-format on
    }

    void hideFontProps()
    {
        hideProperty("fontname");
        hideProperty("fontsize");
    }

    void hideLabel()
    {
        hideProperty("label");
        hideProperty("label_color");
        hideProperty("label_inner");
        hideProperty("label_side");
        hideProperty("label_align");
        hideProperty("label_valign");
        hideProperty("label_margins");

        hideFontProps();
    }

    void hideLabelInner()
    {
        hideProperty("label_inner");
        setPropertyDefaultValue("label_inner", "0");
    }

    void readWrite()
    {
        // clang-format off
        class_addmethod(&pd_ui_class->c_class, (t_method)eobj_write, gensym("write"), A_GIMME, 0);
        class_addmethod(&pd_ui_class->c_class, (t_method)eobj_write, gensym("eobjwriteto"), A_GIMME, 0);
        pd_ui_class->c_widget.w_write = write;

        class_addmethod(&pd_ui_class->c_class, (t_method)eobj_read, gensym("read"), A_GIMME, 0);
        class_addmethod(&pd_ui_class->c_class, (t_method)eobj_read, gensym("eobjreadfrom"), A_GIMME, 0);
        pd_ui_class->c_widget.w_read = read;
        // clang-format on
    }

    void useMouseEvents(int events)
    {
        if (events & UI_MOUSE_DOWN)
            eclass_addmethod(pd_ui_class, UI_METHOD_PTR(mouseDown), str_mousedown, A_GIMME, 0);

        if (events & UI_MOUSE_UP)
            eclass_addmethod(pd_ui_class, UI_METHOD_PTR(mouseUp), str_mouseup, A_GIMME, 0);

        if (events & UI_MOUSE_DRAG)
            eclass_addmethod(pd_ui_class, UI_METHOD_PTR(mouseDrag), str_mousedrag, A_GIMME, 0);

        if (events & UI_MOUSE_ENTER)
            eclass_addmethod(pd_ui_class, UI_METHOD_PTR(mouseEnter), str_mouseenter, A_GIMME, 0);

        if (events & UI_MOUSE_LEAVE)
            eclass_addmethod(pd_ui_class, UI_METHOD_PTR(mouseLeave), str_mouseleave, A_GIMME, 0);

        if (events & UI_MOUSE_MOVE)
            eclass_addmethod(pd_ui_class, UI_METHOD_PTR(mouseMove), str_mousemove, A_GIMME, 0);

        if (events & UI_MOUSE_DBL_CLICK)
            eclass_addmethod(pd_ui_class, UI_METHOD_PTR(dblClick), str_dblclick, A_GIMME, 0);

        if (events & UI_MOUSE_WHEEL)
            eclass_addmethod(pd_ui_class, UI_METHOD_PTR(mouseWheel), str_mousewheel, A_GIMME, 0);
    }

    void outputMouseEvents(MouseEventsOutput t = MouseEventsOutput::DEFAULT_OFF)
    {
        addBoolProperty("mouse_events", _("Output mouse events"),
            t == MouseEventsOutput::DEFAULT_ON,
            &UI::prop_mouse_events, _("Basic"));
    }

    void useKeys()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(key), str_key, A_GIMME, 0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(keyFilter), str_keyfilter, A_GIMME, 0);
    }

    void useBang()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(onBang), str_bang, A_GIMME, 0);
    }

    void useFloat()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(onFloat), str_float, A_FLOAT, 0);
    }

    void useSymbol()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(onSymbol), str_symbol, A_SYMBOL, 0);
    }

    void useList()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(onList), str_list, A_GIMME, 0);
    }

    void useAny()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(onAny), str_anything, A_GIMME, 0);
    }

    void useData()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(onData), str_list, A_GIMME, 0);
    }

    void useDrawCallbacks()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(create), STR_WIDGET_CREATE, A_CANT, 0);
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(erase), STR_WIDGET_ERASE, A_CANT, 0);
    }

    void useAnnotations()
    {
        eclass_addmethod(pd_ui_class, UI_METHOD_PTR(annotate), SymbolTable::instance().s_annotate_fn, A_CANT, 0);
    }

    void addMethod(const char* name, listMethodPtr m)
    {
        addMethod(gensym(name), m);
    }

    void addMethod(t_symbol* name, listMethodPtr m)
    {
        list_map[name] = m;
        eclass_addmethod(pd_ui_class, reinterpret_cast<t_typ_method>(customMethodList), name, A_GIMME, 0);
    }

    void addMethod(const char* name, floatMethodPtr m)
    {
        addMethod(gensym(name), m);
    }

    void addMethod(t_symbol* name, floatMethodPtr m)
    {
        float_map[name] = m;
        eclass_addmethod(pd_ui_class, reinterpret_cast<t_typ_method>(customMethodFloat), name, A_GIMME, 0);
    }

    void addMethod(const char* name, bangMethodPtr m)
    {
        addMethod(gensym(name), m);
    }

    void addMethod(t_symbol* name, bangMethodPtr m)
    {
        bang_map[name] = m;
        eclass_addmethod(pd_ui_class, reinterpret_cast<t_typ_method>(customMethodBang), name, A_GIMME, 0);
    }

    /**
     * @brief adds boolean property shown as toggle in properties dialog
     * @param name - property name
     * @param label - property display label
     * @param def - default property value
     * @param m - member pointer to property
     * @param category - property category
     */
    void addBoolProperty(const char* name, const char* label, bool def, int UI::*m, const char* category = "Misc")
    {
        eclass_new_attr_typed(pd_ui_class, name, "int", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def ? "1" : "0");
        eclass_attr_style(pd_ui_class, name, str_checkbutton);
        eclass_attr_category(pd_ui_class, name, category);
    }

    /**
     * @brief adds float property shown as number entry in property dialog
     * @param name - property name
     * @param label - property display label
     * @param def - default property value
     * @param m - member pointer to property
     * @param category - property category
     */
    void addFloatProperty(const char* name, const char* label, t_float def, t_float UI::*m, const char* category = "Misc")
    {
        char buf[32];
        snprintf(buf, sizeof(buf) - 1, "%g", def);

        if (std::is_same<t_float, float>::value)
            eclass_new_attr_typed(pd_ui_class, name, "float", 1, 0, offset(m));
        else if (std::is_same<t_float, double>::value)
            eclass_new_attr_typed(pd_ui_class, name, "double", 1, 0, offset(m));

        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, buf);
        eclass_attr_style(pd_ui_class, name, str_number);
        eclass_attr_category(pd_ui_class, name, category);
    }

    /**
     * @brief adds integer property shown as integer spinbox entry in properties dialog
     * @param name - property name
     * @param label - property display label
     * @param def - default property value
     * @param m - member pointer to property
     * @param category - property category
     */
    void addIntProperty(const char* name, const char* label, int def, int UI::*m, const char* category = "Misc")
    {
        char buf[32];
        snprintf(buf, sizeof(buf) - 1, "%d", def);

        eclass_new_attr_typed(pd_ui_class, name, "int", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, buf);
        eclass_attr_style(pd_ui_class, name, str_number);
        eclass_attr_category(pd_ui_class, name, category);
    }

    void addRawProperty(const char* name, const char* type, size_t size, size_t maxsize)
    {
        eclass_new_attr_typed(pd_ui_class, name, type, size, maxsize, 0);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
    }

    /**
     * @brief adds symbol enum property shown as dropout menu in properties dialog
     * @param name - property name
     * @param label - property display label
     * @param def - default value
     * @param m - member pointer to property
     * @param items - list of items (space separated) "a b c"
     * @param cat - category name
     */
    void addMenuProperty(const char* name, const char* label,
        const char* def, t_symbol* UI::*m,
        const char* items, const char* cat = "Misc")
    {
        eclass_new_attr_typed(pd_ui_class, name, "symbol", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def);
        eclass_attr_style(pd_ui_class, name, str_menu);
        eclass_attr_category(pd_ui_class, name, cat);
        eclass_attr_itemlist(pd_ui_class, name, items);
    }

    /**
     * @brief adds RGBA color property shown as color chooser in properties dialog
     * @param name - property name
     * @param label - property display label
     * @param def - default value, like "1.0 0.5 0.3 1", in RGBA format
     * @param m - member pointer to property
     */
    void addColorProperty(const char* name, const char* label, const char* def, t_rgba UI::*m)
    {
        eclass_new_attr_typed(pd_ui_class, name, "float", 4, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def);
        eclass_attr_style(pd_ui_class, name, str_color);
        eclass_attr_category(pd_ui_class, name, "Colors");
    }

    /**
     * @brief adds visible in dialog property with callbacks, but without member pointer
     * @param name - property name
     * @param label - property name
     * @param def - default value
     */
    void addVirtualProperty(const char* name, const char* label, const char* def,
        AtomList (UI::*getter)() const, void (UI::*setter)(const AtomListView&))
    {
        eclass_new_attr_typed(pd_ui_class, name, "symbol", 1, 0, 0);
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def);
        eclass_attr_accessor(pd_ui_class, name, listPropGetter, listPropSetter);
        prop_list_map[gensym(name)] = std::make_pair(getter, setter);
    }

    /**
     * @brief adds hidden float property with callbacks
     * @param name - property name
     * @param getter - member pointer to getter function
     * @param setter - member pointer to setter function
     */
    void addHiddenFloatCbProperty(const char* name,
        t_float (UI::*getter)() const,
        void (UI::*setter)(t_float))
    {
        eclass_new_attr_typed(pd_ui_class, name, "float", 1, 0, 0);
        eclass_attr_invisible(pd_ui_class, name);
        setPropertyAccessor(name, getter, setter);
    }

    /**
     * @brief adds hidden list property with callbacks
     * @param name - property name
     * @param getter - member pointer to getter function
     * @param setter - member pointer to setter function
     */
    void addHiddenListCbProperty(const char* name,
        AtomList (UI::*getter)() const,
        void (UI::*setter)(const AtomList&))
    {
        eclass_new_attr_typed(pd_ui_class, name, "atom", 1, 0, 0);
        eclass_attr_invisible(pd_ui_class, name);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name, const char* label, int def, int UI::*m, const char* category = "Misc")
    {
        char buf[32];
        snprintf(buf, 30, "%d", def);

        eclass_new_attr_typed(pd_ui_class, name, "int", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, buf);
        eclass_attr_style(pd_ui_class, name, str_number);
        eclass_attr_category(pd_ui_class, name, category);
    }

    void addProperty(const char* name, const char* label, bool def, int UI::*m, const char* category = "Misc")
    {
        eclass_new_attr_typed(pd_ui_class, name, "int", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def ? "1" : "0");
        eclass_attr_style(pd_ui_class, name, str_checkbutton);
        eclass_attr_category(pd_ui_class, name, category);
    }

    void addProperty(const char* name, const char* label, const char* def, t_rgba UI::*m)
    {
        eclass_new_attr_typed(pd_ui_class, name, "float", 4, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def);
        eclass_attr_style(pd_ui_class, name, str_color);
        eclass_attr_category(pd_ui_class, name, "Colors");
    }

    void addSymbolProperty(const char* name,
        const char* label,
        const char* def,
        t_symbol* UI::*m,
        const char* category = "Misc")
    {
        eclass_new_attr_typed(pd_ui_class, name, "symbol", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def);
        eclass_attr_style(pd_ui_class, name, str_entry);
        eclass_attr_category(pd_ui_class, name, category);
    }

    void addPathProperty(const char* name,
        const char* label,
        const char* def,
        t_symbol* UI::*m,
        const char* category = "Misc")
    {
        eclass_new_attr_typed(pd_ui_class, name, "symbol", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def);
        eclass_attr_style(pd_ui_class, name, str_path);
        eclass_attr_category(pd_ui_class, name, category);
    }

    void setPropertyMin(const char* name, t_float v)
    {
        eclass_attr_filter_min(pd_ui_class, name, v);
    }

    void setPropertyMax(const char* name, t_float v)
    {
        eclass_attr_filter_max(pd_ui_class, name, v);
    }

    void setPropertyLabel(const char* name, const char* label)
    {
        eclass_attr_label(pd_ui_class, name, label);
    }

    void setPropertyRange(const char* name, t_float min, t_float max)
    {
        setPropertyMin(name, min);
        setPropertyMax(name, max);
    }

    void setPropertySave(const char* name, bool value = true)
    {
        eclass_attr_save(pd_ui_class, name, value);
    }

    void setPropertyDefaultValue(const char* name, const char* def)
    {
        eclass_attr_default(pd_ui_class, name, def);
    }

    void setPropertyStep(const char* name, t_float step)
    {
        eclass_attr_step(pd_ui_class, name, step);
    }

    void hideProperty(const char* name)
    {
        eclass_attr_invisible(pd_ui_class, name);
    }

    void showProperty(const char* name)
    {
        eclass_attr_visible(pd_ui_class, name);
    }

    void setDefaultSize(int w, int h)
    {
        char buf[32];
        snprintf(buf, 30, "%d. %d.", w, h);

        CLASS_ATTR_DEFAULT(pd_ui_class, str_size, buf);
    }

    void setPropertyCategory(const char* name, const char* cat_name)
    {
        eclass_attr_category(pd_ui_class, name, cat_name);
    }

    void addProperty(const char* name, propBoolGet getter, propBoolSet setter = nullptr)
    {
        eclass_new_attr_typed(pd_ui_class, name, "int", 1, 1, 0);
        eclass_attr_style(pd_ui_class, name, str_checkbutton);
        eclass_attr_invisible(pd_ui_class, name);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name, propIntGet getter, propIntSet setter = nullptr)
    {
        eclass_new_attr_typed(pd_ui_class, name, "int", 1, 1, 0);
        eclass_attr_invisible(pd_ui_class, name);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name, propFloatGet getter, propFloatSet setter = nullptr)
    {
        eclass_new_attr_typed(pd_ui_class, name, "float", 1, 1, 0);
        eclass_attr_invisible(pd_ui_class, name);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name, propSymbolGet getter, propSymbolSet setter = nullptr)
    {
        eclass_new_attr_typed(pd_ui_class, name, "symbol", 1, 1, 0);
        eclass_attr_style(pd_ui_class, name, str_entry);
        eclass_attr_invisible(pd_ui_class, name);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name, propAtomGet getter, propAtomSet setter = nullptr)
    {
        eclass_new_attr_typed(pd_ui_class, name, "atom", 1, 1, 0);
        eclass_attr_invisible(pd_ui_class, name);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name, propListGet getter, propListSet setter = nullptr)
    {
        eclass_new_attr_typed(pd_ui_class, name, "atom", 1, 0, 0);
        eclass_attr_invisible(pd_ui_class, name);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name, const char* label,
        const char* def, t_symbol* UI::*m,
        const char* items, const char* cat = "Misc")
    {
        eclass_new_attr_typed(pd_ui_class, name, "symbol", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def);
        eclass_attr_style(pd_ui_class, name, str_menu);
        eclass_attr_category(pd_ui_class, name, cat);
        eclass_attr_itemlist(pd_ui_class, name, items);
    }

    void addPropertyIntMenu(const char* name, const char* label,
        const char* def, int UI::*m,
        const char* items, const char* cat = "Misc")
    {
        eclass_new_attr_typed(pd_ui_class, name, "int", 1, 0, offset(m));
        eclass_attr_label(pd_ui_class, name, label);
        eclass_attr_save(pd_ui_class, name);
        eclass_attr_paint(pd_ui_class, name);
        eclass_attr_default(pd_ui_class, name, def);
        eclass_attr_style(pd_ui_class, name, str_menu);
        eclass_attr_category(pd_ui_class, name, cat);
        eclass_attr_itemlist(pd_ui_class, name, items);
    }

    void setPropertyAccessor(const char* name, propBoolGet getter, propBoolSet setter)
    {
        auto m = static_cast<t_setter_method>(setter != nullptr ? boolPropSetter : nullptr);
        eclass_attr_accessor(pd_ui_class, name, boolPropGetter, m);
        prop_bool_map[gensym(name)] = std::make_pair(getter, setter);
    }

    void setPropertyAccessor(const char* name, propFloatGet getter, propFloatSet setter)
    {
        auto m = static_cast<t_setter_method>(setter != nullptr ? floatPropSetter : nullptr);
        eclass_attr_accessor(pd_ui_class, name, floatPropGetter, m);
        prop_float_map[gensym(name)] = std::make_pair(getter, setter);
    }

    void setPropertyAccessor(const char* name, propIntGet getter, propIntSet setter)
    {
        auto m = static_cast<t_setter_method>(setter != nullptr ? intPropSetter : nullptr);
        eclass_attr_accessor(pd_ui_class, name, intPropGetter, m);
        prop_int_map[gensym(name)] = std::make_pair(getter, setter);
    }

    void setPropertyAccessor(const char* name, propSymbolGet getter, propSymbolSet setter)
    {
        auto m = static_cast<t_setter_method>(setter != nullptr ? symbolPropSetter : nullptr);
        eclass_attr_accessor(pd_ui_class, name, symbolPropGetter, m);
        prop_symbol_map[gensym(name)] = std::make_pair(getter, setter);
    }

    void setPropertyAccessor(const char* name, propAtomGet getter, propAtomSet setter)
    {
        auto m = static_cast<t_setter_method>(setter != nullptr ? atomPropSetter : nullptr);
        eclass_attr_accessor(pd_ui_class, name, atomPropGetter, m);
        prop_atom_map[gensym(name)] = std::make_pair(getter, setter);
    }
    void setPropertyAccessor(const char* name, propListGet getter, propListSet setter)
    {
        auto m = static_cast<t_setter_method>(setter != nullptr ? listPropSetter : nullptr);
        eclass_attr_accessor(pd_ui_class, name, listPropGetter, m);
        prop_list_map[gensym(name)] = std::make_pair(getter, setter);
    }

    void setPropertyRedirect(const char* name)
    {
        eclass_attr_redirect(pd_ui_class, name, (t_gotfn)listPropRedirector);
    }

    void setPropertyUnits(const char* name, const char* value)
    {
        setPropertyUnits(gensym(name), gensym(value));
    }

    void setPropertyUnits(t_symbol* name, t_symbol* value)
    {
        eclass_attr_units(pd_ui_class, name, value);
    }

    void addAlias(const char* name)
    {
        ObjectInfoStorage::instance().addAlias(name, &pd_ui_class->c_class, reinterpret_cast<t_newmethod>(alloc));
    }

public:
    static void paint(UI* z)
    {
        z->paint();
    }

    static void create(UI* z)
    {
        z->create();
    }

    static void erase(UI* z)
    {
        z->erase();
    }

    static const char* annotate(UI* z, int type, int idx)
    {
        switch (type) {
        case 0:
            return z->annotateOutlet(idx);
        case 1:
            return z->annotateInlet(idx);
        default:
            return nullptr;
        }
    }

    template <class T>
    static size_t offset(T UI::*m)
    {
        static UI ui;

        char* v1 = (char*)static_cast<void*>(&(ui.*m));
        char* v0 = (char*)static_cast<void*>(&ui);
        return v1 - v0;
    }

    static void outputMouse(UI* z, t_symbol* s, bool value)
    {
        if (z->outputMouseEvents() && !z->outlets().empty()) {
            t_atom a;
            SETFLOAT(&a, value ? 1 : 0);
            outlet_anything(z->outlets().front(), s, 1, &a);
        }
    }

    static void mouseMove(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        static t_symbol* SYM = gensym("@mouse_move");

        updateMousePos(pt);

        outputMouse(z, SYM, true);
        z->onMouseMove(view, pt, modifiers);
        outputMouse(z, SYM, false);
    }

    static void mouseDown(UI* z, t_object* view, t_pt pt, t_pt abs_pt, long modifiers)
    {
        static t_symbol* SYM = gensym("@mouse_down");

        updateMousePos(pt);

        outputMouse(z, SYM, true);
        z->onMouseDown(view, pt, abs_pt, modifiers);
        outputMouse(z, SYM, false);
    }

    static void mouseUp(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        static t_symbol* SYM = gensym("@mouse_up");

        updateMousePos(pt);

        outputMouse(z, SYM, true);
        z->onMouseUp(view, pt, modifiers);
        outputMouse(z, SYM, false);
    }

    static void mouseDrag(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        static t_symbol* SYM = gensym("@mouse_drag");

        updateMousePos(pt);

        outputMouse(z, SYM, true);
        z->onMouseDrag(view, pt, modifiers);
        outputMouse(z, SYM, false);
    }

    static void mouseLeave(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        static t_symbol* SYM = gensym("@mouse_leave");

        // invalidate mouse pointer coord on mouseLeave to prevent mouseWheel handle
        // when mouse is outside of widget
        updateMousePos({ std::numeric_limits<decltype(t_pt::x)>::max(), std::numeric_limits<decltype(t_pt::y)>::max() });

        outputMouse(z, SYM, true);
        z->onMouseLeave(view, pt, modifiers);
        outputMouse(z, SYM, false);
    }

    static void mouseEnter(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        static t_symbol* SYM = gensym("@mouse_enter");

        updateMousePos(pt);

        outputMouse(z, SYM, true);
        z->onMouseEnter(view, pt, modifiers);
        outputMouse(z, SYM, false);
    }

    static void mouseWheel(UI* z, t_pt pt, long modifiers, t_float delta)
    {
        z->onMouseWheel(mouse_pos_, modifiers, delta);
    }

    static void updateMousePos(const t_pt& pt)
    {
        mouse_pos_ = pt;
    }

    static void dblClick(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        z->onDblClick(view, pt, modifiers);
    }

    static void key(UI* z, t_symbol*, int k, long modifiers)
    {
        z->onKey(k, modifiers);
    }

    static void keyFilter(UI* z, t_symbol*, int k, long modifiers)
    {
        z->onKeyFilter(k, modifiers);
    }

    static void notify(void* x, t_symbol* prop_name, t_symbol* action_name)
    {
        auto* z = static_cast<UI*>(x);

        if (action_name == s_attr_modified) {
            if (use_presets && prop_name == gensym(PROP_PRESET_NAME))
                z->handlePresetNameChange();

            z->onPropChange(prop_name);
        }
    }

    static void okSize(void* x, ::t_rect* newrect)
    {
        ((UI*)x)->okSize(newrect);
    }

    static void write(void* x, const char* fname)
    {
        ((UI*)x)->write(fname);
    }

    static void read(void* x, const char* fname)
    {
        ((UI*)x)->read(fname);
    }

    static void onZoom(void* x, t_float zoom)
    {
        ((UI*)x)->onZoom(zoom);
    }

    static void processPopup(UI* z, t_symbol* menu_name, long itemIdx, t_pt rel_pos)
    {
        z->onPopup(menu_name, itemIdx, rel_pos);
    }

    static void showPopup(UI* z, t_pt pos, t_pt abs_pos)
    {
        z->showPopup(pos, abs_pos);
    }

    static void onBang(UI* z)
    {
        z->onBang();
    }

    static void onFloat(UI* z, t_float f)
    {
        z->onFloat(f);
    }

    static void onSymbol(UI* z, t_symbol* s)
    {
        z->onSymbol(s);
    }

    static void onList(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        // compile time method type check
        using ListMethodType = void (UI::*)(const AtomListView&);
        ListMethodType mptr = &UI::onList;

        z->onList(AtomListView(argv, argc));
    }

    static void onAny(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        // compile time method type check
        using AnyMethodType = void (UI::*)(t_symbol*, const AtomListView&);
        AnyMethodType mptr = &UI::onAny;

        z->onAny(s, AtomListView(argv, argc));
    }

    static int checkPresetIndex(UI* z, int argc, t_atom* argv)
    {
        int idx = 0;
        if (argc > 0) {
            if (argv->a_type != A_FLOAT) {
                pd_error(z->asPd(), "[%s] invalid preset index", z->name()->s_name);
                return -1;
            }

            idx = atom_getint(argv);
        }

        return idx;
    }

    static void loadPreset(void* x, t_symbol*, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        int idx = checkPresetIndex(z, argc, argv);

        if (idx < 0)
            return;

        z->loadPreset(idx);
    }

    static void interpPreset(void* x, t_symbol*, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        t_float idx = 0;
        if (argc > 0) {
            if (argv->a_type != A_FLOAT) {
                pd_error(z->asPd(), "[%s] invalid preset index", z->name()->s_name);
                return;
            }

            idx = atom_getfloat(argv);
        }

        if (z->hasPresetInterp())
            z->interpPreset(idx);
        else
            z->loadPreset(static_cast<int>(idx));
    }

    static void storePreset(void* x, t_symbol*, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        int idx = checkPresetIndex(z, argc, argv);

        if (idx < 0)
            return;

        z->storePreset(idx);
    }

    static void clearPreset(void* x, t_symbol*, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        int idx = checkPresetIndex(z, argc, argv);

        if (idx < 0)
            return;

        z->clearPreset(idx);
    }

    static void customMethodList(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        auto it = list_map.find(s);
        if (it == list_map.end()) {
            pd_error(z->asPdObject(), "[%s] unknown method: %s", z->name()->s_name, s->s_name);
            return;
        }

        listMethodPtr m = it->second;

        if (!m) {
            pd_error(z->asPdObject(), "[%s] NULL function pointer: %s", z->name()->s_name, s->s_name);
            return;
        }

        (z->*m)(AtomListView(argv, argc));
    }

    static void customMethodFloat(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        if (argc < 1)
            return;

        auto it = float_map.find(s);
        if (it == float_map.end()) {
            pd_error(z->asPdObject(), "[%s] unknown method: %s", z->name()->s_name, s->s_name);
            return;
        }

        floatMethodPtr m = it->second;

        if (!m) {
            pd_error(z->asPdObject(), "[%s] NULL function pointer: %s", z->name()->s_name, s->s_name);
            return;
        }

        (z->*m)(atom_getfloat(argv));
    }

    static void customMethodBang(UI* z, t_symbol* s, int, t_atom*)
    {
        auto it = bang_map.find(s);
        if (it == bang_map.end()) {
            pd_error(z->asPdObject(), "[%s] unknown method: %s", z->name()->s_name, s->s_name);
            return;
        }

        bangMethodPtr m = it->second;

        if (!m) {
            pd_error(z->asPdObject(), "[%s] NULL function pointer: %s", z->name()->s_name, s->s_name);
            return;
        }

        (z->*m)();
    }

    static void setDrawParams(UI* z, t_edrawparams* params)
    {
        z->setDrawParams(params);
    }

    static bool boolPropGetter(void* x, t_eattr* attr, int* argc, t_atom** argv)
    {
        auto z = static_cast<UI*>(x);
        auto it = prop_bool_map.find(attr->name);
        if (it == prop_bool_map.end())
            return false;

        auto m = it->second.first;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] non-readable property: @%s", z->name()->s_name, attr->name->s_name);
            argc = 0;
            *argv = 0;
            return false;
        }

        *argc = 1;
        *argv = (t_atom*)getbytes(sizeof(t_atom));
        atom_setfloat(*argv, (int)(z->*m)());
        return true;
    }

    static bool boolPropSetter(void* x, t_eattr* attr, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        auto it = prop_bool_map.find(attr->name);
        if (it == prop_bool_map.end())
            return false;

        if (argc < 1)
            return false;

        auto m = it->second.second;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] readonly property: @%s", z->name()->s_name, attr->name->s_name);
            return false;
        }

        int value = -1;
        if (argv->a_type == A_SYMBOL) {
            auto sym = atom_getsymbol(argv);
            if (sym == gensym("true")) {
                value = 1;
            } else if (sym == gensym("false")) {
                value = 0;
            } else {
                pd_error(z->asPdObject(),
                    "[%s] @%s: bool value (0|1|true|false) expected, got '%s'",
                    z->name()->s_name, attr->name->s_name, sym->s_name);
                return false;
            }
        } else if (argv->a_type == A_FLOAT) {
            value = (atom_getfloat(argv) != 0);
        } else {
            pd_error(z->asPdObject(),
                "[%s] @%s: bool value (0|1|true|false) expected",
                z->name()->s_name, attr->name->s_name);
            return false;
        }

        (z->*m)(value);
        return true;
    }

    static bool intPropGetter(void* x, t_eattr* attr, int* argc, t_atom** argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_int_map.find(attr->name);
        if (it == prop_int_map.end())
            return 1;

        auto m = it->second.first;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] non-readable property: @%s", z->name()->s_name, attr->name->s_name);
            argc = 0;
            *argv = 0;
            return false;
        }

        *argc = 1;
        *argv = (t_atom*)getbytes(sizeof(t_atom));
        atom_setfloat(*argv, (z->*m)());
        return true;
    }

    static bool intPropSetter(void* x, t_eattr* attr, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_int_map.find(attr->name);
        if (it == prop_int_map.end())
            return false;

        if (argc < 1)
            return false;

        auto m = it->second.second;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] readonly property: @%s", z->name()->s_name, attr->name->s_name);
            return false;
        }

        (z->*m)(atom_getfloat(argv));
        return true;
    }

    static bool floatPropGetter(void* x, t_eattr* attr, int* argc, t_atom** argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_float_map.find(attr->name);
        if (it == prop_float_map.end())
            return false;

        propFloatGet m = it->second.first;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] non-readable property: @%s", z->name()->s_name, attr->name->s_name);
            argc = 0;
            *argv = 0;
            return 1;
        }

        *argc = 1;
        *argv = (t_atom*)getbytes(sizeof(t_atom));
        atom_setfloat(*argv, (z->*m)());
        return true;
    }

    static bool floatPropSetter(void* x, t_eattr* attr, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_float_map.find(attr->name);
        if (it == prop_float_map.end())
            return false;

        if (argc < 1)
            return false;

        propFloatSet m = it->second.second;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] readonly property: @%s", z->name()->s_name, attr->name->s_name);
            return false;
        }

        (z->*m)(atom_getfloat(argv));
        return true;
    }

    static bool symbolPropGetter(void* x, t_eattr* attr, int* argc, t_atom** argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_symbol_map.find(attr->name);
        if (it == prop_symbol_map.end())
            return false;

        auto m = it->second.first;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] non-readable property: @%s", z->name()->s_name, attr->name->s_name);
            argc = 0;
            *argv = 0;
            return 1;
        }

        *argc = 1;
        *argv = (t_atom*)getbytes(sizeof(t_atom));
        auto sym = (z->*m)();
        SETSYMBOL(argv[0], sym);
        return true;
    }

    static bool symbolPropSetter(void* x, t_eattr* attr, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_symbol_map.find(attr->name);
        if (it == prop_symbol_map.end())
            return false;

        if (argc < 1)
            return false;

        auto m = it->second.second;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] readonly property: @%s", z->name()->s_name, attr->name->s_name);
            return false;
        }

        if (argv->a_type != A_SYMBOL) {
            pd_error(z->asPdObject(), "[%s] @%s: symbol expected", z->name()->s_name, attr->name->s_name);
            return false;
        }

        (z->*m)(atom_getsymbol(argv));
        return true;
    }

    static bool atomPropGetter(void* x, t_eattr* attr, int* argc, t_atom** argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_atom_map.find(attr->name);
        if (it == prop_atom_map.end())
            return false;

        auto m = it->second.first;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] non-readable property: @%s", z->name()->s_name, attr->name->s_name);
            argc = 0;
            *argv = 0;
            return false;
        }

        *argc = 1;
        *argv = (t_atom*)getbytes(sizeof(t_atom));
        *argv[0] = (z->*m)().atom();
        return true;
    }

    static bool atomPropSetter(void* x, t_eattr* attr, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_atom_map.find(attr->name);
        if (it == prop_atom_map.end())
            return false;

        if (argc < 1)
            return false;

        auto m = it->second.second;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] readonly property: @%s", z->name()->s_name, attr->name->s_name);
            return false;
        }

        (z->*m)(Atom(*argv));
        return true;
    }

    static bool listPropGetter(void* x, t_eattr* attr, int* argc, t_atom** argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr) {
            LIB_ERR << "NULL attr argument";
            return false;
        }

        auto it = prop_list_map.find(attr->name);
        if (it == prop_list_map.end()) {
            LIB_ERR << "attr getter not found: " << attr->name->s_name;
            return false;
        }

        propListGet m = it->second.first;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] non-readable property: @%s", z->name()->s_name, attr->name->s_name);
            argc = 0;
            *argv = 0;
            return false;
        }

        AtomList res = (z->*m)();

        *argc = res.size();
        *argv = (t_atom*)copybytes(res.toPdData(), sizeof(t_atom) * res.size());
        return true;
    }

    static bool listPropSetter(void* x, t_eattr* attr, int argc, t_atom* argv)
    {
        auto z = static_cast<UI*>(x);
        if (!attr)
            return false;

        auto it = prop_list_map.find(attr->name);
        if (it == prop_list_map.end())
            return 1;

        propListSet m = it->second.second;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] readonly property: @%s", z->name()->s_name, attr->name->s_name);
            return false;
        }

        (z->*m)(AtomListView(argv, argc));
        return true;
    }

    static void onData(UI* z, t_symbol*, int argc, t_atom* argv)
    {
        if (argc == 1 && Atom::is_data(argv)) {
            z->onData(Atom(*argv));
        } else {
            z->onList(AtomListView(argv, argc));
        }
    }

    static void listPropRedirector(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        // compile time method type check
        using PropertyMethodType = void (UI::*)(t_symbol*, const AtomListView&);
        PropertyMethodType mptr = &UI::onProperty;

        (z->*mptr)(s, AtomListView(argv, argc));
    }

public:
    static t_eclass* pd_ui_class;
    static bool use_presets;
    static long flags;
    static BangMethodMap bang_map;
    static FloatMethodMap float_map;
    static ListMethodMap list_map;
    static BoolPropertyMap prop_bool_map;
    static IntPropertyMap prop_int_map;
    static FloatPropertyMap prop_float_map;
    static SymbolPropertyMap prop_symbol_map;
    static AtomPropertyMap prop_atom_map;
    static ListPropertyMap prop_list_map;
    static t_pt mouse_pos_;
};

template <class UI>
t_eclass* UIObjectFactory<UI>::pd_ui_class = nullptr;

template <class UI>
bool UIObjectFactory<UI>::use_presets = false;

template <class UI>
long UIObjectFactory<UI>::flags = 0;

template <class UI>
t_pt UIObjectFactory<UI>::mouse_pos_;

template <class UI>
typename UIObjectFactory<UI>::BangMethodMap UIObjectFactory<UI>::bang_map;

template <class UI>
typename UIObjectFactory<UI>::FloatMethodMap UIObjectFactory<UI>::float_map;

template <class UI>
typename UIObjectFactory<UI>::ListMethodMap UIObjectFactory<UI>::list_map;

template <class UI>
typename UIObjectFactory<UI>::BoolPropertyMap UIObjectFactory<UI>::prop_bool_map;

template <class UI>
typename UIObjectFactory<UI>::IntPropertyMap UIObjectFactory<UI>::prop_int_map;

template <class UI>
typename UIObjectFactory<UI>::FloatPropertyMap UIObjectFactory<UI>::prop_float_map;

template <class UI>
typename UIObjectFactory<UI>::SymbolPropertyMap UIObjectFactory<UI>::prop_symbol_map;

template <class UI>
typename UIObjectFactory<UI>::AtomPropertyMap UIObjectFactory<UI>::prop_atom_map;

template <class UI>
typename UIObjectFactory<UI>::ListPropertyMap UIObjectFactory<UI>::prop_list_map;

}

#endif // CEAMMC_NEW_GUI_H
