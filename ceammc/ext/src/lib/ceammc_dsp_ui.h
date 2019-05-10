#ifndef CEAMMC_DSP_UI_H
#define CEAMMC_DSP_UI_H

#include "ceammc_atomlist.h"
#include "ceammc_cicm.h"
#include "ceammc_externals.h"
#include "ceammc_ui.h"

#include <string>
#include <unordered_map>
#include <utility>

namespace ceammc {

#define UI_DSP_METHOD_PTR(m) reinterpret_cast<t_typ_method>(&UIDspFactory<UI>::m)

template <class UI>
class UIDspFactory {
    std::string name_;

public:
    typedef void (UI::*bangMethodPtr)();
    typedef void (UI::*floatMethodPtr)(t_float);
    typedef void (UI::*listMethodPtr)(const AtomList&);
    typedef float (UI::*propFloatGet)() const;
    typedef void (UI::*propFloatSet)(float);
    typedef AtomList (UI::*propListGet)() const;
    typedef void (UI::*propListSet)(const AtomList&);
    typedef std::pair<propFloatGet, propFloatSet> propertyFloatAccess;
    typedef std::pair<propListGet, propListSet> propertyListAccess;

    typedef std::unordered_map<t_symbol*, bangMethodPtr> BangMethodMap;
    typedef std::unordered_map<t_symbol*, floatMethodPtr> FloatMethodMap;
    typedef std::unordered_map<t_symbol*, listMethodPtr> ListMethodMap;
    typedef std::unordered_map<t_symbol*, propertyFloatAccess> FloatPropertyMap;
    typedef std::unordered_map<t_symbol*, propertyListAccess> ListPropertyMap;

public:
    UIDspFactory(const char* name, long fl = EBOX_GROWINDI, int pd_flags = 0)
        : name_(name)
    {
        flags = fl;
        if (pd_class) {
            pd_error(0, "class already initialized: %s", name);
            return;
        }

        pd_class = eclass_new(name_.c_str(),
            UI_DSP_METHOD_PTR(alloc),
            UI_DSP_METHOD_PTR(free),
            sizeof(UI), pd_flags, A_GIMME, 0);

        if (!pd_class) {
            pd_error(0, "can't allocate class: %s", name);
            return;
        }

        eclass_dspinit(pd_class);
        eclass_guiinit(pd_class, flags);
        setupMethods();
        setupAttributes();
        register_ui_external(&pd_class->c_class);
    }

    ~UIDspFactory()
    {
        eclass_attr_sort(pd_class);
        eclass_register(CLASS_BOX, pd_class);
    }

    static UI* alloc(t_symbol* s, int argc, t_atom* argv)
    {
        void* mem = eobj_new(pd_class);
        t_binbuf* d = binbuf_via_atoms(argc, argv);

        if (mem && d) {
            UI* x = new (mem) UI();

            ebox_new(x->asEBox(), 0 | flags);
            x->dspInit();
            ebox_attrprocess_viabinbuf(x, d);
            ebox_ready((t_ebox*)x);
            binbuf_free(d);

            x->init(s, AtomList(argc, argv), use_presets);
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

    void setupMethods()
    {
        // clang-format off
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(dsp),           "dsp",           A_NULL,  0);
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(paint),         "paint",         A_GIMME, 0);
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(notify),        "notify",        A_GIMME, 0);
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(okSize),        "oksize",        A_GIMME, 0);
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(onPopup),       "popup",         A_GIMME, 0);
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(setDrawParams), "getdrawparams", A_NULL,  0);
        // clang-format on
    }

    void setupAttributes()
    {
        CLASS_ATTR_INVISIBLE(pd_class, "fontweight", 1);
        CLASS_ATTR_INVISIBLE(pd_class, "fontslant", 1);

        // clang-format off
        // background / border color
        addColorProperty(PROP_BACKGROUND_COLOR,
                         _("Background Color"),
                         DEFAULT_BACKGROUND_COLOR,
                         &UI::prop_color_background);

        addColorProperty(PROP_BORDER_COLOR,
                         _("Border Color"),
                         DEFAULT_BORDER_COLOR,
                         &UI::prop_color_border);

        addColorProperty(PROP_LABEL_COLOR,
                         _("Label Color"),
                         DEFAULT_LABEL_COLOR,
                         &UI::prop_color_label);

        // default
        CLASS_ATTR_DEFAULT              (pd_class, "size", 0, "45. 15.");
        // clang-format on
    }

    void readWrite()
    {
        // clang-format off
        eclass_addmethod(pd_class, UI_METHOD_PTR(write),         "write",         A_GIMME,  0);
        eclass_addmethod(pd_class, UI_METHOD_PTR(read),          "read",          A_GIMME,  0);
        // clang-format on
    }

    void usePresets()
    {
        // clang-format off

        UIDspFactory::use_presets = true;

        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(loadPreset),      "load",          A_GIMME,  0);
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(storePreset),     "store",         A_GIMME,  0);
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(clearPreset),     "clear",         A_GIMME,  0);

        CLASS_ATTR_SYMBOL   (pd_class, PROP_PRESET_NAME, 0, t_ebox, b_objpreset_id);
        CLASS_ATTR_DEFAULT  (pd_class, PROP_PRESET_NAME, 0, "(null)");
        CLASS_ATTR_SAVE     (pd_class, PROP_PRESET_NAME, 0);
        CLASS_ATTR_CATEGORY (pd_class, PROP_PRESET_NAME, 0, _("Basic"));
        CLASS_ATTR_LABEL    (pd_class, PROP_PRESET_NAME, 0, _("Preset Name"));
        CLASS_ATTR_ACCESSORS(pd_class, PROP_PRESET_NAME, NULL, ebox_set_presetid);

        // clang-format on
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
        setPropertyDefaultValue("label_inner", "outer");
    }

    void hideFontProps()
    {
        CLASS_ATTR_INVISIBLE(pd_class, "fontname", 1);
        CLASS_ATTR_INVISIBLE(pd_class, "fontsize", 1);
    }

    void useMouseEvents(int events)
    {
        if (events & UI_MOUSE_DOWN)
            eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(mouseDown), "mousedown", A_GIMME, 0);

        if (events & UI_MOUSE_UP)
            eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(mouseUp), "mouseup", A_GIMME, 0);

        if (events & UI_MOUSE_DRAG)
            eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(mouseDrag), "mousedrag", A_GIMME, 0);

        if (events & UI_MOUSE_ENTER)
            eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(mouseEnter), "mouseenter", A_GIMME, 0);

        if (events & UI_MOUSE_LEAVE)
            eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(mouseLeave), "mouseleave", A_GIMME, 0);

        if (events & UI_MOUSE_MOVE)
            eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(mouseMove), "mousemove", A_GIMME, 0);

        if (events & UI_MOUSE_DBL_CLICK)
            eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(dblClick), "dblclick", A_GIMME, 0);

        if (events & UI_MOUSE_WHEEL)
            eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(mouseWheel), "mousewheel", A_GIMME, 0);
    }

    void useKeys()
    {
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(key), "key", A_GIMME, 0);
    }

    void useBang()
    {
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(onBang), "bang", A_GIMME, 0);
    }

    void useFloat()
    {
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(onFloat), "float", A_FLOAT, 0);
    }

    void useSymbol()
    {
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(onSymbol), "symbol", A_SYMBOL, 0);
    }

    void useList()
    {
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(onList), "list", A_GIMME, 0);
    }

    void useAny()
    {
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(onAny), "anything", A_GIMME, 0);
    }

    void useData()
    {
        eclass_addmethod(pd_class, UI_DSP_METHOD_PTR(onData), "list", A_GIMME, 0);
    }

    void addMethod(const char* name, listMethodPtr m)
    {
        addMethod(gensym(name), m);
    }

    void addMethod(t_symbol* name, listMethodPtr m)
    {
        list_map[name] = m;
        eclass_addmethod(pd_class, reinterpret_cast<t_typ_method>(customMethodList), name->s_name, A_GIMME, 0);
    }

    void addMethod(const char* name, floatMethodPtr m)
    {
        addMethod(gensym(name), m);
    }

    void addMethod(t_symbol* name, floatMethodPtr m)
    {
        float_map[name] = m;
        eclass_addmethod(pd_class, reinterpret_cast<t_typ_method>(customMethodFloat), name->s_name, A_GIMME, 0);
    }

    void addMethod(const char* name, bangMethodPtr m)
    {
        addMethod(gensym(name), m);
    }

    void addMethod(t_symbol* name, bangMethodPtr m)
    {
        bang_map[name] = m;
        eclass_addmethod(pd_class, reinterpret_cast<t_typ_method>(customMethodBang), name->s_name, A_GIMME, 0);
    }

    /**
     * @brief adds boolean property
     * @param name - property name
     * @param label - property display label
     * @param def - default property value
     * @param m - member pointer to property
     * @param category - property category
     */
    void addBoolProperty(const char* name, const char* label, bool def, int UI::*m, const char* category = "Misc")
    {
        eclass_new_attr_typed(pd_class, name, "int", 1, 0, 0, offset(m));
        eclass_attr_label(pd_class, name, 0, label);
        eclass_attr_save(pd_class, name, 0);
        eclass_attr_paint(pd_class, name, 0);
        eclass_attr_default(pd_class, name, 0, def ? "1" : "0");
        eclass_attr_style(pd_class, name, 0, "onoff");
        eclass_attr_category(pd_class, name, 0, category);
    }

    /**
     * @brief adds float property
     * @param name - property name
     * @param label - property display label
     * @param def - default property value
     * @param m - member pointer to property
     * @param category - property category
     */
    void addFloatProperty(const char* name, const char* label, float def, float UI::*m, const char* category = "Misc")
    {
        char buf[32];
        snprintf(buf, 30, "%g", def);

        eclass_new_attr_typed(pd_class, name, "float", 1, 0, 0, offset(m));
        eclass_attr_label(pd_class, name, 0, label);
        eclass_attr_save(pd_class, name, 0);
        eclass_attr_paint(pd_class, name, 0);
        eclass_attr_default(pd_class, name, 0, buf);
        eclass_attr_style(pd_class, name, 0, "number");
        eclass_attr_category(pd_class, name, 0, category);
    }

    /**
     * @brief adds integer property
     * @param name - property name
     * @param label - property display label
     * @param def - default property value
     * @param m - member pointer to property
     * @param category - property category
     */
    void addIntProperty(const char* name, const char* label, int def, int UI::*m, const char* category = "Misc")
    {
        char buf[32];
        snprintf(buf, 30, "%d", def);

        eclass_new_attr_typed(pd_class, name, "int", 1, 0, 0, offset(m));
        eclass_attr_label(pd_class, name, 0, label);
        eclass_attr_save(pd_class, name, 0);
        eclass_attr_paint(pd_class, name, 0);
        eclass_attr_default(pd_class, name, 0, buf);
        eclass_attr_style(pd_class, name, 0, "number");
        eclass_attr_category(pd_class, name, 0, category);
    }

    /**
     * @brief adds RGBA color property
     * @param name - property name
     * @param label - property display label
     * @param def - default value, like "1.0 0.5 0.3 1", in RGBA format
     * @param m - member pointer to property
     */
    void addColorProperty(const char* name, const char* label, const char* def, t_rgba UI::*m)
    {
        eclass_new_attr_typed(pd_class, name, "float", 4, 0, 0, offset(m));
        eclass_attr_label(pd_class, name, 0, label);
        eclass_attr_save(pd_class, name, 0);
        eclass_attr_paint(pd_class, name, 0);
        eclass_attr_default(pd_class, name, 0, def);
        eclass_attr_style(pd_class, name, 0, "color");
        eclass_attr_category(pd_class, name, 0, "Colors");
    }

    void addProperty(const char* name,
        float (UI::*getter)() const,
        void (UI::*setter)(float))
    {
        eclass_new_attr_typed(pd_class, name, "float", 1, 0, 0, 0);
        eclass_attr_invisible(pd_class, name, 0);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name,
        AtomList (UI::*getter)() const,
        void (UI::*setter)(const AtomList&))
    {
        eclass_new_attr_typed(pd_class, name, "atom", 1, 0, 0, 0);
        eclass_attr_invisible(pd_class, name, 0);
        setPropertyAccessor(name, getter, setter);
    }

    void addProperty(const char* name, const char* label,
        const char* def, t_symbol* UI::*m,
        const char* items, const char* cat = "Misc")
    {
        eclass_new_attr_typed(pd_class, name, "symbol", 1, 0, 0, offset(m));
        eclass_attr_label(pd_class, name, 0, label);
        eclass_attr_save(pd_class, name, 0);
        eclass_attr_paint(pd_class, name, 0);
        eclass_attr_default(pd_class, name, 0, def);
        eclass_attr_style(pd_class, name, 0, "menu");
        eclass_attr_category(pd_class, name, 0, cat);
        eclass_attr_itemlist(pd_class, name, 0, items);
    }

    void addVirtualProperty(const char* name, const char* label, const char* def,
        AtomList (UI::*getter)() const, void (UI::*setter)(const AtomList&))
    {
        eclass_new_attr_typed(pd_class, name, "symbol", 1, 0, 0, 0);
        eclass_attr_label(pd_class, name, 0, label);
        eclass_attr_save(pd_class, name, 0);
        eclass_attr_paint(pd_class, name, 0);
        eclass_attr_default(pd_class, name, 0, def);
        eclass_attr_accessor(pd_class, name, (t_err_method)listPropGetter, (t_err_method)listPropSetter);
        prop_list_map[gensym(name)] = std::make_pair(getter, setter);
    }

    void setPropertyMin(const char* name, float v)
    {
        eclass_attr_filter_min(pd_class, name, v);
    }

    void setPropertyMax(const char* name, float v)
    {
        eclass_attr_filter_max(pd_class, name, v);
    }

    void setPropertyRange(const char* name, float min, float max)
    {
        setPropertyMin(name, min);
        setPropertyMax(name, max);
    }

    void setPropertySave(const char* name, bool value = true)
    {
        eclass_attr_save(pd_class, name, 0, value);
    }

    void setPropertyDefaultValue(const char* name, const char* def)
    {
        eclass_attr_default(pd_class, name, 0, def);
    }

    void hideProperty(const char* name)
    {
        eclass_attr_invisible(pd_class, name, 0);
    }

    void showProperty(const char* name)
    {
        eclass_attr_visible(pd_class, name, 0);
    }

    void setDefaultSize(int w, int h)
    {
        char buf[32];
        snprintf(buf, 30, "%d. %d.", w, h);

        CLASS_ATTR_DEFAULT(pd_class, "size", 0, buf);
    }

    void setPropertyAccessor(const char* name, float (UI::*getter)() const, void (UI::*setter)(float))
    {
        eclass_attr_accessor(pd_class, name, (t_err_method)floatPropGetter, (t_err_method)floatPropSetter);
        prop_float_map[gensym(name)] = std::make_pair(getter, setter);
    }

    void setPropertyAccessor(const char* name, AtomList (UI::*getter)() const, void (UI::*setter)(const AtomList&))
    {
        eclass_attr_accessor(pd_class, name, (t_err_method)listPropGetter, (t_err_method)listPropSetter);
        prop_list_map[gensym(name)] = std::make_pair(getter, setter);
    }

    void setPropertyRedirect(const char* name)
    {
        eclass_attr_redirect(pd_class, name, (t_gotfn)listPropRedirector);
    }

    void addAlias(const char* name)
    {
        class_addcreator(reinterpret_cast<t_newmethod>(alloc), gensym(name), A_GIMME, A_NULL);
    }

public:
    static void dsp(UI* z, t_object* dsp,
        short* count, double samplerate, long maxvectorsize, long flags)
    {
        z->dspOn(samplerate, maxvectorsize);
        object_method(dsp, gensym("dsp_add"), z, UI_DSP_METHOD_PTR(dspProcess), 0, NULL);
    }

    static void dspProcess(UI* z, t_object* dsp,
        t_sample** ins, long n_ins,
        t_sample** outs, long n_outs,
        long sampleframes, long flags, void* up)
    {
        z->dspProcess(ins, n_ins, outs, n_outs, sampleframes);
    }

    static void paint(UI* z)
    {
        z->paint();
    }

    template <class T>
    static size_t offset(T UI::*m)
    {
        static UI ui;

        char* v1 = (char*)static_cast<void*>(&(ui.*m));
        char* v0 = (char*)static_cast<void*>(&ui);
        return v1 - v0;
    }

    static void mouseMove(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        z->onMouseMove(view, pt, modifiers);
    }

    static void mouseDown(UI* z, t_object* view, t_pt pt, t_pt abs_pt, long modifiers)
    {
        z->onMouseDown(view, pt, abs_pt, modifiers);
    }

    static void mouseUp(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        z->onMouseUp(view, pt, modifiers);
    }

    static void mouseDrag(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        z->onMouseDrag(view, pt, modifiers);
    }

    static void mouseLeave(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        z->onMouseLeave(view, pt, modifiers);
    }

    static void mouseEnter(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        z->onMouseEnter(view, pt, modifiers);
    }

    static void mouseWheel(UI* z, t_object* view, t_pt pt, long modifiers, double delta)
    {
        z->onMouseWheel(view, pt, modifiers, delta);
    }

    static void dblClick(UI* z, t_object* view, t_pt pt, long modifiers)
    {
        z->onDblClick(view, pt, modifiers);
    }

    static void key(UI* z, t_symbol*, int k, t_atom*)
    {
        z->onKey(k);
    }

    static void notify(UI* z, t_symbol* prop_name, t_symbol* action_name)
    {
        if (action_name == s_attr_modified) {
            if (use_presets && prop_name == gensym(PROP_PRESET_NAME))
                z->handlePresetNameChange();

            z->onPropChange(prop_name);
        }
    }

    static void write(UI* z, const char* fname)
    {
        z->write(fname);
    }

    static void read(UI* z, const char* fname)
    {
        z->read(fname);
    }

    static void okSize(UI* z, ::t_rect* newrect)
    {
        z->okSize(newrect);
    }

    static void onPopup(UI* z, t_symbol* menu_name, long itemIdx)
    {
        z->onPopup(menu_name, itemIdx);
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
        z->onList(AtomList(argc, argv));
    }

    static void onAny(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        z->onAny(s, AtomList(argc, argv));
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

    static void loadPreset(UI* z, t_symbol*, int argc, t_atom* argv)
    {
        int idx = checkPresetIndex(z, argc, argv);

        if (idx < 0)
            return;

        z->loadPreset(idx);
    }

    static void storePreset(UI* z, t_symbol*, int argc, t_atom* argv)
    {
        int idx = checkPresetIndex(z, argc, argv);

        if (idx < 0)
            return;

        z->storePreset(idx);
    }

    static void clearPreset(UI* z, t_symbol*, int argc, t_atom* argv)
    {
        int idx = checkPresetIndex(z, argc, argv);

        if (idx < 0)
            return;

        z->clearPreset(idx);
    }

    static void customMethodList(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        typename ListMethodMap::iterator it = list_map.find(s);
        if (it == list_map.end()) {
            pd_error(z->asPdObject(), "[%s] unknown method: %s", z->name()->s_name, s->s_name);
            return;
        }

        listMethodPtr m = it->second;

        if (!m) {
            pd_error(z->asPdObject(), "[%s] NULL function pointer: %s", z->name()->s_name, s->s_name);
            return;
        }

        (z->*m)(AtomList(argc, argv));
    }

    static void customMethodFloat(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        if (argc < 1)
            return;

        typename FloatMethodMap::iterator it = float_map.find(s);
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
        typename BangMethodMap::iterator it = bang_map.find(s);
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

    static t_pd_err floatPropGetter(UI* z, t_eattr* attr, int* argc, t_atom** argv)
    {
        typename FloatPropertyMap::iterator it = prop_float_map.find(attr->name);
        if (it == prop_float_map.end())
            return 1;

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
        return 0;
    }

    static t_pd_err floatPropSetter(UI* z, t_eattr* attr, int argc, t_atom* argv)
    {
        typename FloatPropertyMap::iterator it = prop_float_map.find(attr->name);
        if (it == prop_float_map.end())
            return 1;

        if (argc < 1)
            return 2;

        propFloatSet m = it->second.second;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] readonly property: @%s", z->name()->s_name, attr->name->s_name);
            return 1;
        }

        (z->*m)(atom_getfloat(argv));
        return 0;
    }

    static t_pd_err listPropGetter(UI* z, t_eattr* attr, int* argc, t_atom** argv)
    {
        typename ListPropertyMap::iterator it = prop_list_map.find(attr->name);
        if (it == prop_list_map.end())
            return 1;

        propListGet m = it->second.first;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] non-readable property: @%s", z->name()->s_name, attr->name->s_name);
            argc = 0;
            *argv = 0;
            return 1;
        }

        AtomList res = (z->*m)();

        *argc = res.size();
        *argv = (t_atom*)copybytes(res.toPdData(), sizeof(t_atom) * res.size());
        return 0;
    }

    static t_pd_err listPropSetter(UI* z, t_eattr* attr, int argc, t_atom* argv)
    {
        typename ListPropertyMap::iterator it = prop_list_map.find(attr->name);
        if (it == prop_list_map.end())
            return 1;

        propListSet m = it->second.second;

        if (m == 0) {
            pd_error(z->asPdObject(), "[%s] readonly property: @%s", z->name()->s_name, attr->name->s_name);
            return 1;
        }

        (z->*m)(AtomList(argc, argv));
        return 0;
    }

    static void onData(UI* z, t_symbol*, int argc, t_atom* argv)
    {
        AtomList l(argc, argv);
        if (l.size() == 1 && l[0].isData()) {
            DataPtr ptr(l[0]);
            if (ptr.isValid()) {
                z->onData(ptr);
            } else {
                DataDesc desc = l[0].getData();
                LIB_ERR << "can't get data with type=" << desc.type << " and id=" << desc.id;
            }
        } else {
            z->onList(l);
        }
    }

    static void listPropRedirector(UI* z, t_symbol* s, int argc, t_atom* argv)
    {
        z->onProperty(s, AtomList(argc, argv));
    }

public:
    static t_eclass* pd_class;
    static bool use_presets;
    static long flags;
    static BangMethodMap bang_map;
    static FloatMethodMap float_map;
    static ListMethodMap list_map;
    static FloatPropertyMap prop_float_map;
    static ListPropertyMap prop_list_map;
};

template <class UI>
t_eclass* UIDspFactory<UI>::pd_class = 0;

template <class UI>
bool UIDspFactory<UI>::use_presets = false;

template <class UI>
long UIDspFactory<UI>::flags = 0;

template <class UI>
typename UIDspFactory<UI>::BangMethodMap UIDspFactory<UI>::bang_map;

template <class UI>
typename UIDspFactory<UI>::FloatMethodMap UIDspFactory<UI>::float_map;

template <class UI>
typename UIDspFactory<UI>::ListMethodMap UIDspFactory<UI>::list_map;

template <class UI>
typename UIDspFactory<UI>::FloatPropertyMap UIDspFactory<UI>::prop_float_map;

template <class UI>
typename UIDspFactory<UI>::ListPropertyMap UIDspFactory<UI>::prop_list_map;
}

#endif // CEAMMC_DSP_UI_H
