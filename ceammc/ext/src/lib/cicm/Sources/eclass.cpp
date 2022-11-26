/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eclass.h"
#include "ceammc.h"
#include "ceammc_impl.h"
#include "ceammc_log.h"
#include "ceammc_syms.h"
#include "ebox.h"
#include "ecommon.h"
#include "egraphics.h"
#include "eobj.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#include "fmt/format.h"
#pragma clang diagnostic pop

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#define _(msg) msg

enum CategoryType {
    CAT_BASE = 0,
    CAT_COLOR = 100,
    CAT_LABEL = 200,
    CAT_BOUNDS = 300,
    CAT_MAIN = 400,
    CAT_MIDI = 500,
    CAT_MISC = 600
};

typedef struct tcl_version {
    t_object x_obj;
    char minor;
    char major;
} t_tcl_version;

static void eclass_properties_dialog(t_eclass* c);
static void ewidget_init(t_eclass* c);
static void eclass_attr_ceammc_setter(t_object* x, t_symbol* s, size_t argc, t_atom* argv);
static t_tcl_version* tcl_version_new();

static t_class* tcl_version_class = nullptr;
static t_tcl_version* tcl_version_instance = nullptr;

t_tcl_version* tcl_version_new()
{
    t_tcl_version* x = reinterpret_cast<t_tcl_version*>(pd_new(tcl_version_class));
    if (x) {
        x->minor = 0;
        x->major = 0;
    }

    return x;
}

void tcl_version_set(t_tcl_version* x, t_float v)
{
    float int_part;
    float frac_part = modff(v, &int_part);

    x->major = static_cast<char>(int_part);
    x->minor = static_cast<char>(roundf(frac_part * 10));

    if (x->minor != 6)
        post("[ceammc] TCL/Tk version is: %d.%d", x->major, x->minor);
}

void tcl_version_init()
{
    if (tcl_version_class == 0) {
        tcl_version_class = class_new(gensym("_tcl.version"),
            reinterpret_cast<t_newmethod>(tcl_version_new), 0,
            sizeof(t_tcl_version), 0, A_NULL);

        class_addmethod(tcl_version_class, reinterpret_cast<t_method>(tcl_version_set),
            gensym("tcl_version"), A_DEFFLOAT, A_NULL);

        tcl_version_instance = tcl_version_new();
        pd_bind(&tcl_version_instance->x_obj.te_g.g_pd, gensym("tcl_version"));
        sys_gui("pdsend \"tcl_version tcl_version $tk_version\"\n");
    }
}

std::pair<int, int> eclass_tcl_version()
{
    if (!tcl_version_instance)
        return { 0, 0 };
    else
        return { tcl_version_instance->major, tcl_version_instance->minor };
}

t_eclass* eclass_new(const char* name, t_typ_method newm, t_typ_method freem, size_t size, int flags, t_atomtype arg1, int arg2)
{
    t_class* pd = class_new(gensym(name),
        reinterpret_cast<t_newmethod>(newm),
        reinterpret_cast<t_method>(freem),
        size, flags, arg1, arg2);

    if (!pd) {
        bug("PureData Memory allocation failed for the class %s.", name);
        return 0;
    }

    // we are using same size to prevent PD from memset to 0 resized memory.
    // we should do this way because some versions of PD have different sizeof(t_class)
    t_eclass* c = static_cast<t_eclass*>(resizebytes(pd, sizeof(t_eclass), sizeof(t_eclass)));

    if (c) {
        tcl_version_init();
        epd_init();
        memset(&c->c_widget, 0, sizeof(t_ewidget));
        c->c_dsp = false;
        c->c_box = false;
        c->c_attr = nullptr;
        c->c_nattr = 0;
    } else {
        bug("Memory allocation failed for the class %s.", name);
        return 0;
    }

    return c;
}

void eclass_init(t_eclass* c, long flags)
{
    eclass_guiinit(c, flags);
}

void eclass_guiinit(t_eclass* c, long /*flags*/)
{
    ewidget_init(c);
    c->c_box = 1;

    // DEFAULT ATTRIBUTES //
    CLASS_ATTR_FLOAT_ARRAY(c, "size", t_ebox, b_rect.width, 2);
    CLASS_ATTR_SYMBOL(c, "fontname", t_ebox, b_font.c_family);
    CLASS_ATTR_SYMBOL(c, "fontweight", t_ebox, b_font.c_weight);
    CLASS_ATTR_SYMBOL(c, "fontslant", t_ebox, b_font.c_slant);
    CLASS_ATTR_INT(c, "fontsize", t_ebox, b_font.c_sizereal);
    CLASS_ATTR_SYMBOL(c, "receive", t_ebox, b_receive_id);
    CLASS_ATTR_SYMBOL(c, "send", t_ebox, b_send_id);
    CLASS_ATTR_SYMBOL(c, "label", t_ebox, b_label);
    CLASS_ATTR_INT(c, "label_inner", t_ebox, label_inner);
    CLASS_ATTR_SYMBOL(c, "label_side", t_ebox, label_side);
    CLASS_ATTR_SYMBOL(c, "label_align", t_ebox, label_align);
    CLASS_ATTR_SYMBOL(c, "label_valign", t_ebox, label_valign);
    CLASS_ATTR_INT_ARRAY(c, "label_margins", t_ebox, label_margins, 2);

    CLASS_ATTR_DEFAULT(c, "size", "100. 100.");
    CLASS_ATTR_FILTER_MIN(c, "size", 4);
    CLASS_ATTR_SAVE(c, "size");
    CLASS_ATTR_PAINT(c, "size");
    CLASS_ATTR_CATEGORY(c, "size", _("Basic"));
    CLASS_ATTR_LABEL(c, "size", _("Patching Size"));
    CLASS_ATTR_ACCESSORS(c, "size", nullptr, ebox_size_set);

    CLASS_ATTR_INT(c, "pinned", t_ebox, b_pinned);
    CLASS_ATTR_DEFAULT(c, "pinned", "0");
    CLASS_ATTR_FILTER_CLIP(c, "pinned", 0, 1);
    CLASS_ATTR_SAVE(c, "pinned");
    CLASS_ATTR_CATEGORY(c, "pinned", _("Basic"));
    CLASS_ATTR_LABEL(c, "pinned", _("Pinned"));
    CLASS_ATTR_STYLE(c, "pinned", "onoff");

    CLASS_ATTR_DEFAULT(c, "fontname", "Helvetica");
    CLASS_ATTR_SAVE(c, "fontname");
    CLASS_ATTR_PAINT(c, "fontname");
    CLASS_ATTR_CATEGORY(c, "fontname", _("Label"));
    CLASS_ATTR_LABEL(c, "fontname", _("Font Name"));
    CLASS_ATTR_ACCESSORS(c, "fontname", NULL, ebox_set_font);
    CLASS_ATTR_STYLE(c, "fontname", "menu");
    CLASS_ATTR_ITEMS(c, "fontname", "Helvetica Monaco Courier Times DejaVu");

    CLASS_ATTR_DEFAULT(c, "fontweight", _("normal"));
    CLASS_ATTR_SAVE(c, "fontweight");
    CLASS_ATTR_PAINT(c, "fontweight");
    CLASS_ATTR_CATEGORY(c, "fontweight", _("Label"));
    CLASS_ATTR_LABEL(c, "fontweight", _("Font Weight"));
    CLASS_ATTR_ACCESSORS(c, "fontweight", NULL, ebox_set_fontweight);
    CLASS_ATTR_STYLE(c, "fontweight", "menu");
    CLASS_ATTR_ITEMS(c, "fontweight", "normal bold");

    CLASS_ATTR_DEFAULT(c, "fontslant", "roman");
    CLASS_ATTR_SAVE(c, "fontslant");
    CLASS_ATTR_PAINT(c, "fontslant");
    CLASS_ATTR_CATEGORY(c, "fontslant", _("Label"));
    CLASS_ATTR_LABEL(c, "fontslant", _("Font Slant"));
    CLASS_ATTR_ACCESSORS(c, "fontslant", NULL, ebox_set_fontslant);
    CLASS_ATTR_STYLE(c, "fontslant", "menu");
    CLASS_ATTR_ITEMS(c, "fontslant", "roman italic");

    CLASS_ATTR_DEFAULT(c, "fontsize", "11");
    CLASS_ATTR_FILTER_MIN(c, "fontsize", 4);
    CLASS_ATTR_SAVE(c, "fontsize");
    CLASS_ATTR_PAINT(c, "fontsize");
    CLASS_ATTR_CATEGORY(c, "fontsize", _("Label"));
    CLASS_ATTR_LABEL(c, "fontsize", _("Font Size"));
    CLASS_ATTR_ACCESSORS(c, "fontsize", NULL, ebox_set_fontsize);
    CLASS_ATTR_STYLE(c, "fontsize", "number");

    CLASS_ATTR_DEFAULT(c, "receive", "(null)");
    CLASS_ATTR_ACCESSORS(c, "receive", NULL, ebox_set_receiveid);
    CLASS_ATTR_SAVE(c, "receive");
    CLASS_ATTR_CATEGORY(c, "receive", _("Basic"));
    CLASS_ATTR_LABEL(c, "receive", _("Receive Symbol"));

    CLASS_ATTR_DEFAULT(c, "send", "(null)");
    CLASS_ATTR_ACCESSORS(c, "send", NULL, ebox_set_sendid);
    CLASS_ATTR_SAVE(c, "send");
    CLASS_ATTR_CATEGORY(c, "send", _("Basic"));
    CLASS_ATTR_LABEL(c, "send", _("Send Symbol"));

    CLASS_ATTR_DEFAULT(c, "label", "(null)");
    CLASS_ATTR_ACCESSORS(c, "label", NULL, ebox_set_label);
    CLASS_ATTR_SAVE(c, "label");
    CLASS_ATTR_CATEGORY(c, "label", _("Label"));
    CLASS_ATTR_LABEL(c, "label", _("Text"));

    CLASS_ATTR_DEFAULT(c, "label_align", "left");
    CLASS_ATTR_ACCESSORS(c, "label_align", NULL, ebox_set_label_align);
    CLASS_ATTR_SAVE(c, "label_align");
    CLASS_ATTR_CATEGORY(c, "label_align", _("Label"));
    CLASS_ATTR_LABEL(c, "label_align", _("Horizontal align"));
    CLASS_ATTR_STYLE(c, "label_align", "menu");
    CLASS_ATTR_ITEMS(c, "label_align", "left center right");

    CLASS_ATTR_DEFAULT(c, "label_valign", "top");
    CLASS_ATTR_ACCESSORS(c, "label_valign", NULL, ebox_set_label_valign);
    CLASS_ATTR_SAVE(c, "label_valign");
    CLASS_ATTR_CATEGORY(c, "label_valign", _("Label"));
    CLASS_ATTR_LABEL(c, "label_valign", _("Vertical align"));
    CLASS_ATTR_STYLE(c, "label_valign", "menu");
    CLASS_ATTR_ITEMS(c, "label_valign", "top center bottom");

    CLASS_ATTR_DEFAULT(c, "label_side", "top");
    CLASS_ATTR_ACCESSORS(c, "label_side", NULL, ebox_set_label_side);
    CLASS_ATTR_SAVE(c, "label_side");
    CLASS_ATTR_CATEGORY(c, "label_side", _("Label"));
    CLASS_ATTR_LABEL(c, "label_side", _("Attach side"));
    CLASS_ATTR_STYLE(c, "label_side", "menu");
    CLASS_ATTR_ITEMS(c, "label_side", "left top right bottom");

    CLASS_ATTR_DEFAULT(c, "label_inner", "0");
    CLASS_ATTR_ACCESSORS(c, "label_inner", NULL, ebox_set_label_position);
    CLASS_ATTR_SAVE(c, "label_inner");
    CLASS_ATTR_CATEGORY(c, "label_inner", _("Label"));
    CLASS_ATTR_LABEL(c, "label_inner", _("Inner position"));
    CLASS_ATTR_STYLE(c, "label_inner", "onoff");
    CLASS_ATTR_FILTER_CLIP(c, "label_inner", 0, 1);

    CLASS_ATTR_DEFAULT(c, "label_margins", "0 0");
    CLASS_ATTR_SAVE(c, "label_margins");
    CLASS_ATTR_PAINT(c, "label_margins");
    CLASS_ATTR_CATEGORY(c, "label_margins", _("Label"));
    CLASS_ATTR_LABEL(c, "label_margins", _("Margins"));
    CLASS_ATTR_ACCESSORS(c, "label_margins", NULL, ebox_set_label_margins);

    // GUI always need this methods //
    t_class* cc = &c->c_class;

    using namespace ceammc;
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_attr_dump), sym_dump(), A_NULL, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_output_all_attrs), gensym(STR_GET_ALL_PROPS), A_NULL, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_dialog), sym_dialog(), A_GIMME, 0);

    class_addmethod(cc, reinterpret_cast<t_method>(ebox_mouse_enter), sym_mouseenter(), A_NULL, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_mouse_leave), sym_mouseleave(), A_NULL, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_mouse_move), sym_mousemove(), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_mouse_down), sym_mousedown(), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_mouse_up), sym_mouseup(), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_mouse_rightclick), sym_rightclick(), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);

    class_addmethod(cc, reinterpret_cast<t_method>(ebox_pos), sym_pos(), A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_vis), sym_vis(), A_DEFFLOAT, 0);
    class_addmethod(cc, reinterpret_cast<t_method>(ebox_setzoom), sym_zoom(), A_CANT, 0);

    class_setwidget(cc, (t_widgetbehavior*)&c->c_widget);
    class_setsavefn(cc, reinterpret_cast<t_savefn>(eobj_save));
}

void eclass_dspinit(t_eclass* c)
{
    t_class* cls = &c->c_class;
    c->c_dsp = true;

    using namespace ceammc;
    class_addmethod(cls, reinterpret_cast<t_method>(eobj_dsp), sym_dsp(), A_CANT, 0);
    class_addmethod(cls, reinterpret_cast<t_method>(eobj_dsp_add), sym_dsp_add(), A_NULL, 0);
    //    class_addmethod(cc, reinterpret_cast<t_method>(eobj_dsp_add), gensym(SYM_DSP_ADD64), A_NULL, 0);
}

static t_pd_err is_cicm(t_eobj* /*x*/)
{
    return 1;
}

static int eclass_attr_iface_getter(t_object* x, t_symbol* prop_name, int* argc, t_atom** argv)
{
    char buf[128];
    strncpy(buf, &prop_name->s_name[1], sizeof(buf) - 1); // skip first @
    buf[sizeof(buf) - 1] = '\0';
    return eclass_attr_getter(x, gensym(buf), argc, argv);
}

static int eclass_attr_iface_setter(t_object* x, t_symbol* prop_name, int argc, t_atom* argv)
{
    char buf[128];
    strncpy(buf, &prop_name->s_name[1], sizeof(buf) - 1); // skip first @
    buf[sizeof(buf) - 1] = '\0';
    return eclass_attr_setter(x, gensym(buf), argc, argv);
}

t_pd_err eclass_register(t_symbol* /*name*/, t_eclass* c)
{
    t_class* cc = &c->c_class;

    if (c->c_dsp) {
        long diff = 0;

        if (c->c_box)
            diff = calcoffset(t_edspbox, d_dsp.d_float);
        else
            diff = calcoffset(t_edspobj, d_dsp.d_float);

        class_domainsignalin(cc, int(diff));
    }

    if (c->c_nattr) {
        eclass_properties_dialog(c);
        class_setpropertiesfn(cc, reinterpret_cast<t_propertiesfn>(ebox_properties));
    }

    class_addmethod(cc, reinterpret_cast<t_method>(is_cicm), s_iscicm, A_CANT, 0);

    // props
    ceammc::ceammc_class_add_propget_fn(cc, eclass_attr_iface_getter);
    ceammc::ceammc_class_add_propset_fn(cc, eclass_attr_iface_setter);

    return 0;
}

void eclass_addmethod(t_eclass* c, t_typ_method m, const char* name, t_atomtype type, long /*dummy*/)
{
    eclass_addmethod(c, m, gensym(name), type, 0);
}

void eclass_addmethod(t_eclass* c, t_typ_method m, t_symbol* sname, t_atomtype type, long /*dummy*/)
{
    auto* cls = &c->c_class;
    using namespace ceammc;
    switch (crc32_hash(sname)) {
    case hash_mouseenter:
        c->c_widget.w_mouseenter = reinterpret_cast<t_mouseenter_method>(m);
        break;
    case hash_mouseleave:
        c->c_widget.w_mouseleave = reinterpret_cast<t_mouseleave_method>(m);
        break;
    case hash_mousemove:
        c->c_widget.w_mousemove = reinterpret_cast<t_mousemove_method>(m);
        break;
    case hash_mousedown:
        c->c_widget.w_mousedown = reinterpret_cast<t_mousedown_method>(m);
        break;
    case hash_mousedrag:
        c->c_widget.w_mousedrag = reinterpret_cast<t_mousedrag_method>(m);
        break;
    case hash_mouseup:
        c->c_widget.w_mouseup = reinterpret_cast<t_mouseup_method>(m);
        break;
    case hash_rightclick:
        c->c_widget.w_rightclick = reinterpret_cast<t_rightclick_method>(m);
        break;
    case hash_mousewheel:
        class_addmethod(cls, reinterpret_cast<t_method>(ebox_mouse_wheel),
            sym_mousewheel(), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
        c->c_widget.w_mousewheel = reinterpret_cast<t_mousewheel_method>(m);
        break;
    case hash_dblclick:
        class_addmethod(cls, reinterpret_cast<t_method>(ebox_mouse_dblclick),
            sym_dblclick(), A_GIMME, 0);
        c->c_widget.w_dblclick = reinterpret_cast<t_dblclick_method>(m);
        break;
    case hash_key:
        if (c->c_widget.w_key == nullptr && c->c_widget.w_keyfilter == nullptr)
            class_addmethod(cls, reinterpret_cast<t_method>(ebox_key), sym_key(), A_GIMME, 0);

        c->c_widget.w_key = reinterpret_cast<t_key_method>(m);
        break;
    case hash_keyfilter:
        if (c->c_widget.w_key == nullptr && c->c_widget.w_keyfilter == nullptr)
            class_addmethod(cls, reinterpret_cast<t_method>(ebox_key), sym_key(), A_GIMME, 0);

        c->c_widget.w_keyfilter = reinterpret_cast<t_key_method>(m);
        break;
    case hash_paint:
        c->c_widget.w_paint = reinterpret_cast<t_paint_method>(m);
        break;
    case crc32_constexpr(STR_WIDGET_CREATE):
        c->c_widget.w_create = reinterpret_cast<t_create_method>(m);
        break;
    case crc32_constexpr(STR_WIDGET_ERASE):
        c->c_widget.w_erase = reinterpret_cast<t_erase_method>(m);
        break;
    case hash_notify:
        c->c_widget.w_notify = reinterpret_cast<t_notify_method>(m);
        break;
    case hash_getdrawparams:
        c->c_widget.w_getdrawparameters = reinterpret_cast<t_draw_param_method>(m);
        break;
    case hash_oksize:
        c->c_widget.w_oksize = reinterpret_cast<t_oksize_method>(m);
        break;
    case hash_zoom:
        c->c_widget.w_onzoom = reinterpret_cast<t_zoom_method>(m);
        break;
    case hash_save:
        break;
    case hash_popup:
        class_addmethod(cls, reinterpret_cast<t_method>(eobj_popup),
            sym_popup(), A_SYMBOL, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
        c->c_widget.w_popup = reinterpret_cast<t_popup_method>(m);
        break;
    case hash_dsp:
        c->c_widget.w_dsp = reinterpret_cast<t_dsp_method>(m);
        break;
    case hash_bang:
        class_addbang(cls, reinterpret_cast<t_method>(m));
        break;
    case hash_float:
        class_doaddfloat(cls, reinterpret_cast<t_method>(m));
        break;
    case hash_symbol:
        class_addsymbol(cls, reinterpret_cast<t_method>(m));
        break;
    case hash_list:
        class_addlist(cls, reinterpret_cast<t_method>(m));
        break;
    case hash_anything:
        class_addanything(cls, reinterpret_cast<t_method>(m));
        break;
    case hash_preset:
        CLASS_ATTR_SYMBOL(c, "presetname", t_ebox, b_objpreset_id);
        CLASS_ATTR_DEFAULT(c, "presetname", "(null)");
        CLASS_ATTR_SAVE(c, "presetname");
        CLASS_ATTR_CATEGORY(c, "presetname", _("Basic"));
        CLASS_ATTR_LABEL(c, "presetname", _("Preset Name"));
        CLASS_ATTR_ACCESSORS(c, "presetname", NULL, ebox_set_presetid);
        class_addmethod(cls, reinterpret_cast<t_method>(m), sname, type, 0);
        break;
    default:
        class_addmethod(cls, reinterpret_cast<t_method>(m), sname, type, 0);
        break;
    }
}

static t_symbol* gentype(const char* name)
{
    if (strcmp(name, "float") == 0)
        return &s_float;
    else if (strcmp(name, "symbol") == 0)
        return &s_symbol;
    else
        return gensym(name);
}

void eclass_new_attr_typed(t_eclass* c, const char* attrname, const char* type,
    size_t size, size_t maxsize, size_t offset)
{
    if (size >= 1) {
        t_symbol* name = gensym(attrname);

        for (size_t i = 0; i < c->c_nattr; i++) {
            if (c->c_attr[i]->name == name) {
                pd_error(nullptr, "%s already have %s attribute.", c->c_class.c_name->s_name, attrname);
                return;
            }
        }

        auto attr = static_cast<t_eattr*>(getbytes(sizeof(t_eattr)));
        using namespace ceammc;

        if (attr) {
            attr->name = name;
            attr->type = gentype(type);
            attr->category = c->c_class.c_name;
            attr->label = gensym("");
            attr->style = sym_entry();
            attr->units = &s_;
            attr->order = c->c_nattr + 1;
            attr->save = false;
            attr->paint = false;
            attr->invisible = false;
            attr->offset = offset;
            attr->size = size;
            attr->sizemax = maxsize;
            attr->getter = nullptr;
            attr->setter = nullptr;
            attr->clipped = E_CLIP_NONE;
            attr->minimum = 0;
            attr->maximum = 1;
            attr->step = 1;
            attr->defvals = nullptr;
            attr->itemslist = nullptr;
            attr->itemssize = 0;

            size_t new_sz = (c->c_nattr + 1) * sizeof(t_eattr*);
            t_eattr** attrs = (t_eattr**)resizebytes(c->c_attr, new_sz, new_sz);

            if (attrs) {
                char buf[MAXPDSTRING];
                c->c_attr = attrs;
                c->c_attr[c->c_nattr] = attr;
                sprintf(buf, "@%s", attrname);
                class_addmethod(&c->c_class, reinterpret_cast<t_method>(eclass_attr_ceammc_setter), gensym(buf), A_GIMME, 0);
                sprintf(buf, "@%s?", attrname);
                class_addmethod(&c->c_class, reinterpret_cast<t_method>(eclass_attr_ceammc_getter), gensym(buf), A_GIMME, 0);
                c->c_nattr++;
            } else {
                pd_error(nullptr, "%s can't increase memory for %s attribute.", c->c_class.c_name->s_name, attrname);
            }

        } else {
            pd_error(nullptr, "%s can't allocate memory for %s attribute.", c->c_class.c_name->s_name, attrname);
        }
    } else {
        pd_error(nullptr, "%s %s attribute size is too null.", c->c_class.c_name->s_name, attrname);
    }
}

void eclass_attr_redirect(t_eclass* c, const char* attrname, t_gotfn fn)
{
    char buf[MAXPDSTRING];
    sprintf(buf, "@%s", attrname);
    t_symbol* sel0 = gensym(buf);
    sprintf(buf, "@%s?", attrname);
    t_symbol* sel1 = gensym(buf);

    for (int i = 0; i < c->c_class.c_nmethod; i++) {
        auto mname = ceammc::class_method_name(&c->c_class, i);
        if (mname == sel0 || mname == sel1)
            ceammc::class_method_set_fn(&c->c_class, i, (void*)fn);
    }
}

void eclass_attr_default(t_eclass* c, const char* attrname, const char* value)
{
    auto* sel = gensym(attrname);

    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            c->c_attr[i]->defvals = gensym(value);
            return;
        }
    }

    pd_error(nullptr, "[%s] property not found: %s", c->c_class.c_name->s_name, attrname);
}

void eclass_attr_category(t_eclass* c, const char* attrname, const char* category)
{
    t_symbol* cat = gensym(category);
    t_symbol* sel = gensym(attrname);

    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            c->c_attr[i]->category = cat;

            if (cat == gensym(_("Basic")))
                c->c_attr[i]->order += CAT_BASE;
            else if (cat == gensym(_("Colors")))
                c->c_attr[i]->order += CAT_COLOR;
            else if (cat == gensym(_("Bounds")))
                c->c_attr[i]->order += CAT_BOUNDS;
            else if (cat == gensym(_("Label")))
                c->c_attr[i]->order += CAT_LABEL;
            else if (cat == gensym(_("Main")))
                c->c_attr[i]->order += CAT_MAIN;
            else if (cat == gensym(_("MIDI")))
                c->c_attr[i]->order += CAT_MIDI;
            else
                c->c_attr[i]->order += CAT_MISC;

            return;
        }
    }
}

void eclass_attr_order(t_eclass* c, const char* attrname, const char* order)
{
    t_symbol* sel = gensym(attrname);

    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            int n = atoi(order);
            if (isdigit(n))
                c->c_attr[i]->order = n;

            return;
        }
    }
}

void eclass_attr_label(t_eclass* c, const char* attrname, const char* label)
{
    t_symbol* sel = gensym(attrname);

    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            c->c_attr[i]->label = gensym(label);
            return;
        }
    }
}

void eclass_attr_style(t_eclass* c, const char* attrname, const char* style)
{
    using namespace ceammc;

    auto* sel = gensym(attrname);

    for (size_t i = 0; i < c->c_nattr; i++) {
        auto attr = c->c_attr[i];
        if (attr->name == sel) {
            switch (crc32_hash(style)) {
            case hash_checkbutton:
            case hash_onoff:
                attr->style = sym_checkbutton();
                break;
            case hash_rgb:
            case hash_rgba:
            case hash_color:
                attr->style = sym_color();
                break;
            case hash_number:
            case hash_spinbox:
                attr->style = sym_number();
                break;
            case hash_menu:
                attr->style = sym_menu();
                break;
            case hash_path:
                attr->style = sym_path();
                break;
            case hash_entry:
                attr->style = sym_entry();
                break;
            default:
                LIB_ERR << fmt::format("unknown attribute style @{}: {}", attrname, style);
                break;
            }

            return;
        }
    }
}

static size_t item_count(const char* str, size_t len)
{
    enum State {
        BEGIN,
        SEP,
        OTHER
    };

    auto is_sep = [](char c) { return c == ' ' || c == ','; };

    State st = BEGIN;
    size_t count = 0;

    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        switch (st) {
        case BEGIN:
        case SEP:
            if (!is_sep(c)) {
                st = OTHER;
                count++;
            }
            break;
        case OTHER:
        default:
            st = is_sep(c) ? SEP : OTHER;
            break;
        }
    }

    return count;
}

static void fill_items(const char* str, size_t len, t_symbol** itemlist)
{
    enum State {
        BEGIN,
        SEP,
        OTHER
    };

    auto is_sep = [](char c) { return c == ' ' || c == ','; };
    std::string item;

    State st = BEGIN;
    size_t from = 0;
    size_t n = 0;

    for (size_t i = 0; i <= len; i++) {
        char c = str[i];
        switch (st) {
        case BEGIN:
        case SEP:
            if (!is_sep(c)) {
                st = OTHER;
                from = i;
            }
            break;
        case OTHER:
        default:
            if (is_sep(c) || c == '\0') {
                st = SEP;
                item.assign(str + from, i - from);
                itemlist[n++] = gensym(item.c_str());
            }
            break;
        }
    }
}

void eclass_attr_itemlist(t_eclass* c, const char* attrname, const char* list)
{
    constexpr size_t MAX_ITEMS = 256;
    const auto SYM_ATTR = gensym(attrname);

    for (size_t i = 0; i < c->c_nattr; i++) {
        auto attr = c->c_attr[i];

        // looking for attr
        if (attr->name != SYM_ATTR)
            continue;

        const size_t len = list ? strlen(list) : 0;
        size_t new_size = item_count(list, len);

        const size_t max_items = (attr->sizemax > 0) ? attr->sizemax : MAX_ITEMS;

        if (new_size > max_items) {
            pd_error(nullptr, "[%s] to many property items: %d, clipping to %d", c->c_class.c_name->s_name, (int)new_size, (int)max_items);
            new_size = max_items;
        }

        // non-empty list
        if (new_size > 0) {
            const auto old_size = attr->itemssize;
            const auto old_bsize = old_size * sizeof(t_symbol*);
            const auto new_bsize = new_size * sizeof(t_symbol*);

            // memory allocation
            if (old_size > 0) {
                attr->itemslist = (t_symbol**)resizebytes(attr->itemslist, old_bsize, new_bsize);
                if (attr->itemslist)
                    attr->itemssize = new_size;
                else {
                    pd_error(nullptr, "[%s] can't resize itemlist to size %d", c->c_class.c_name->s_name, (int)new_size);
                    return;
                }
            } else {
                attr->itemslist = (t_symbol**)getbytes(new_bsize);
                if (attr->itemslist)
                    attr->itemssize = new_size;
                else {
                    pd_error(nullptr, "[%s] can't allocate itemlist of size %d", c->c_class.c_name->s_name, (int)new_size);
                    return;
                }
            }

            // memory alloc is ok
            if (attr->itemslist && attr->itemssize) {
                fill_items(list, len, attr->itemslist);
            } else {
                pd_error(nullptr, "[%s] %s error in @%s", c->c_class.c_name->s_name, __FUNCTION__, attrname);
                return;
            }

        } else {
            if (c->c_attr[i]->itemssize)
                freebytes(c->c_attr[i]->itemslist, 0);

            c->c_attr[i]->itemssize = 0;
        }
        return;
    }
}

void eclass_attr_filter_min(t_eclass* c, const char* attrname, t_float value)
{
    auto sel = gensym(attrname);
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            c->c_attr[i]->clipped = static_cast<eclip_flags>(c->c_attr[i]->clipped | E_CLIP_MIN);
            c->c_attr[i]->minimum = value;
            return;
        }
    }
}

void eclass_attr_filter_max(t_eclass* c, const char* attrname, t_float value)
{
    auto sel = gensym(attrname);
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            c->c_attr[i]->clipped = static_cast<eclip_flags>(c->c_attr[i]->clipped | E_CLIP_MAX);
            c->c_attr[i]->maximum = value;
            return;
        }
    }
}

void eclass_attr_step(t_eclass* c, const char* attrname, t_float value)
{
    auto s_attrname = gensym(attrname);
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->step = value;
            return;
        }
    }
}

void eclass_attr_save(t_eclass* c, const char* attrname, bool value)
{
    auto sel = gensym(attrname);
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            c->c_attr[i]->save = value;
            return;
        }
    }
}

void eclass_attr_paint(t_eclass* c, const char* attrname)
{
    auto s_attrname = gensym(attrname);
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->paint = true;
            return;
        }
    }
}

void eclass_attr_invisible(t_eclass* c, const char* attrname)
{
    auto s_attrname = gensym(attrname);
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->invisible = true;
            return;
        }
    }
}

void eclass_attr_accessor(t_eclass* c, const char* attrname, t_getter_method getter, t_setter_method setter)
{
    auto s_attrname = gensym(attrname);
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->getter = getter;
            c->c_attr[i]->setter = setter;
            return;
        }
    }
}

int eclass_attr_getter(t_object* x, t_symbol* s, int* argc, t_atom** argv)
{
    t_ebox* z = (t_ebox*)x;
    t_eclass* c = (t_eclass*)z->b_obj.o_obj.te_g.g_pd;

    if (argc == nullptr) {
        pd_error(x, "[%s] null argc pointer given", class_getname(x->te_pd));
        return 0;
    }

    if (*argv) {
        pd_error(x, "[%s] invalid argv pointer given", class_getname(x->te_pd));
        return 0;
    }

    *argc = 0;

    for (size_t i = 0; i < c->c_nattr; i++) {
        auto attr = c->c_attr[i];

        if (attr->name != s)
            continue;

        char* point = (char*)x + attr->offset;

        if (c->c_attr[i]->sizemax == 0) {
            *argc = (int)attr->size;
        } else {
            point = (char*)x + attr->size;
            *argc = attr->size;
            if (*argc > attr->sizemax)
                *argc = (int)attr->sizemax;
        }

        if (c->c_attr[i]->getter) {
            attr->getter(z, attr, argc, argv);
        } else {
            *argv = (t_atom*)getbytes((size_t)*argc * sizeof(t_atom));
            t_symbol* type = attr->type;

            if (type == s_int) {
                for (int j = 0; j < *argc; j++) {
                    atom_setlong(argv[0] + j, ((int*)point)[j]);
                }
            } else if (type == s_long) {
                for (int j = 0; j < *argc; j++) {
                    atom_setlong(argv[0] + j, ((long*)point)[j]);
                }
            } else if (type == &s_float) {
                for (int j = 0; j < *argc; j++) {
                    atom_setfloat(argv[0] + j, ((float*)point)[j]);
                }
            } else if (type == s_double) {
                for (int j = 0; j < *argc; j++) {
                    atom_setfloat(argv[0] + j, (t_float)(((double*)point)[j]));
                }
            } else if (type == &s_symbol) {
                for (int j = 0; j < *argc; j++) {
                    auto sym = ((t_symbol**)point)[j];
                    if (sym) {
                        atom_setsym(argv[0] + j, gensym(sym->s_name));
                    }
                }
            } else if (type == s_atom) {
                for (int j = 0; j < *argc; j++) {
                    argv[0][j] = ((t_atom*)point)[j];
                }
            } else {
                printf("Unknown property get method: %s\n", type->s_name);
                return 0;
            }
        }

        return 1;
    }

    return 0;
}

static bool request_property(t_object* x, t_symbol* s, std::vector<t_atom>& res)
{
    const size_t len = strlen(s->s_name);
    if (len < 3 || len > MAXPDSTRING) {
        pd_error(x, "[%s] invalid property name", class_getname(x->te_pd));
        return false;
    }

    int argc_ = 0;
    t_atom* argv_ = nullptr;
    char buf[MAXPDSTRING];
    // copy property name without leading '@' char and ending '?' char
    memcpy(buf, s->s_name + 1, len - 2);
    buf[len - 2] = '\0';
    t_symbol* prop_name = gensym(buf);
    // NOTE: argv points to allocated array
    eclass_attr_getter(x, prop_name, &argc_, &argv_);

    if (argc_ > 0) {
        // copy property name without ending '?' char
        memcpy(buf, s->s_name, len - 1);
        buf[len - 1] = '\0';
        t_atom at_name;
        atom_setsym(&at_name, gensym(buf));
        res.push_back(at_name);
        for (int i = 0; i < argc_; i++)
            res.push_back(argv_[i]);

        // free memory allocated in eclass_attr_getter()
        freebytes(argv_, 0);
        return true;
    } else {
        memcpy(buf, s->s_name, len - 1);
        buf[len - 1] = '\0';
        pd_error(x, "[%s] unknown property: %s", class_getname(x->te_pd), buf);
        return false;
    }
}

void eclass_attr_ceammc_getter(t_object* x, t_symbol* s, int argc, t_atom* argv)
{
    auto z = reinterpret_cast<t_ebox*>(x);
    if (!z->b_obj.o_obj.te_outlet) {
        pd_error(x, "[%s] can't get property: class has no outlets.", class_getname(x->te_pd));
        return;
    }

    // single request
    if (argc < 1) {
        std::vector<t_atom> res;
        if (!request_property(x, s, res))
            return;

        outlet_anything(z->b_obj.o_obj.te_outlet, res[0].a_w.w_symbol, res.size() - 1, res.data() + 1);
    } else {
        // multiple request
        std::vector<t_atom> res;
        request_property(x, s, res);
        for (int i = 0; i < argc; i++) {
            t_atom* a = &argv[i];
            if (atom_gettype(a) != A_SYMBOL)
                continue;

            if (!request_property(x, atom_getsymbol(a), res))
                continue;
        }

        if (res.empty())
            return;

        outlet_anything(z->b_obj.o_obj.te_outlet, res[0].a_w.w_symbol, res.size() - 1, res.data() + 1);
    }
}

static void eclass_attr_ceammc_setter(t_object* x, t_symbol* s, size_t argc, t_atom* argv)
{
    const size_t len = strlen(s->s_name);
    if (len < 2 || len > MAXPDSTRING) {
        pd_error(x, "[%s] invalid property name", class_getname(x->te_pd));
        return;
    }

    char buf[MAXPDSTRING];
    // copy property name without leading '@' char
    memcpy(buf, s->s_name + 1, len);
    buf[len] = '\0';
    t_symbol* prop_name = gensym(buf);
    eclass_attr_setter(x, prop_name, argc, argv);
}

bool ebox_attr_long_setter(t_ebox* x, t_eattr* a, t_float value, size_t idx, t_eattr_op op)
{
    const auto* xclass = x->b_obj.o_obj.te_g.g_pd;
    const size_t N = (a->sizemax == 0) ? a->size : a->sizemax;

    if (idx >= N) {
        pd_error(x, "[%s][%s] invalid value index: %d",
            class_getname(xclass), a->name->s_name, (int)idx);
        return false;
    }

    const auto type = a->type;
    if (a->type != s_int && a->type != s_long) {
        pd_error(x, "[%s][@%s] not a int property",
            eobj_getclassname(&x->b_obj)->s_name, a->name->s_name);
        return false;
    }

    auto* ptr = reinterpret_cast<int8_t*>(x) + a->offset;
    long cur_val = 0;
    if (type == s_int) {
        auto* pval = reinterpret_cast<int*>(ptr);
        cur_val = pval[idx];
    } else if (type == s_long) {
        auto* pval = reinterpret_cast<long*>(ptr);
        cur_val = pval[idx];
    } else {
        return false;
    }

    long new_val = cur_val;
    switch (op) {
    case EATTR_OP_ASSIGN:
        new_val = value;
        break;
    case EATTR_OP_ADD:
        new_val += value;
        break;
    case EATTR_OP_SUB:
        new_val -= value;
        break;
    case EATTR_OP_MUL:
        new_val *= value;
        break;
    case EATTR_OP_DIV:
        if (value == 0)
            return false;

        new_val /= value;
        break;
    default:
        return false;
    }

    if (a->clipped & E_CLIP_MINMAX && (new_val < a->minimum || new_val > a->maximum)) {
        pd_error(x, "[%s][%s] expecting value in [%f-%f] range, got: %ld",
            class_getname(xclass), a->name->s_name, a->minimum, a->maximum, new_val);
        new_val = a->minimum;
    } else if (a->clipped & E_CLIP_MIN && new_val < a->minimum) {
        pd_error(x, "[%s][%s] value >= %f expected, got: %ld",
            class_getname(xclass), a->name->s_name, a->minimum, new_val);
        new_val = a->minimum; // ???
        return false;
    } else if (a->clipped & E_CLIP_MAX && new_val > a->maximum) {
        pd_error(x, "[%s][%s] value <= %f expected, got: %ld",
            class_getname(xclass), a->name->s_name, a->maximum, new_val);
        new_val = a->maximum;
    }

    if (type == s_int) {
        auto* pval = reinterpret_cast<int*>(ptr);
        pval[idx] = new_val;
        return true;
    } else if (type == s_long) {
        auto* pval = reinterpret_cast<long*>(ptr);
        pval[idx] = new_val;
        return true;
    } else {
        return false;
    }
}

bool ebox_attr_float_setter(t_ebox* x, t_eattr* a, t_float value, size_t idx, t_eattr_op op)
{
    const auto* xclass = x->b_obj.o_obj.te_g.g_pd;
    const size_t N = (a->sizemax == 0) ? a->size : a->sizemax;

    if (idx >= N) {
        pd_error(x, "[%s][%s] invalid value index: %d",
            class_getname(xclass), a->name->s_name, (int)idx);
        return false;
    }

    const auto type = a->type;
    if (a->type != &s_float && a->type != s_double) {
        pd_error(x, "[%s][@%s] not a float property",
            eobj_getclassname(&x->b_obj)->s_name, a->name->s_name);
        return false;
    }

    auto* ptr = reinterpret_cast<int8_t*>(x) + a->offset;
    double cur_val = 0;
    if (type == &s_float) {
        auto* pval = reinterpret_cast<float*>(ptr);
        cur_val = pval[idx];
    } else if (type == s_double) {
        auto* pval = reinterpret_cast<double*>(ptr);
        cur_val = pval[idx];
    } else {
        return false;
    }

    double new_val = cur_val;
    switch (op) {
    case EATTR_OP_ASSIGN:
        new_val = value;
        break;
    case EATTR_OP_ADD:
        new_val += value;
        break;
    case EATTR_OP_SUB:
        new_val -= value;
        break;
    case EATTR_OP_MUL:
        new_val *= value;
        break;
    case EATTR_OP_DIV:
        if (value == 0)
            return false;

        new_val /= value;
        break;
    default:
        return false;
    }

    if (a->clipped & E_CLIP_MINMAX && (new_val < a->minimum || new_val > a->maximum)) {
        pd_error(x, "[%s][%s] expecting value in [%f-%f] range, got: %f",
            class_getname(xclass), a->name->s_name, a->minimum, a->maximum, new_val);
        new_val = a->minimum;
    } else if (a->clipped & E_CLIP_MIN && new_val < a->minimum) {
        pd_error(x, "[%s][%s] value >= %f expected, got: %f",
            class_getname(xclass), a->name->s_name, a->minimum, new_val);
        new_val = a->minimum; // ???
        return false;
    } else if (a->clipped & E_CLIP_MAX && new_val > a->maximum) {
        pd_error(x, "[%s][%s] value <= %f expected, got: %f",
            class_getname(xclass), a->name->s_name, a->maximum, new_val);
        new_val = a->maximum;
    }

    if (type == &s_float) {
        auto* pval = reinterpret_cast<float*>(ptr);
        pval[idx] = new_val;
        return true;
    } else if (type == s_double) {
        auto* pval = reinterpret_cast<double*>(ptr);
        pval[idx] = new_val;
        return true;
    } else {
        return false;
    }
}

static t_eattr_op atom2op(t_atom* a)
{
    if (atom_gettype(a) == A_SYMBOL) {
        auto ch = atom_getsymbol(a)->s_name;

        if (ch[1] == '\0') {
            switch (ch[0]) {
            case '+':
                return EATTR_OP_ADD;
            case '-':
                return EATTR_OP_SUB;
            case '*':
                return EATTR_OP_MUL;
            case '/':
                return EATTR_OP_DIV;
            default:
                break;
            }
        }
    }

    return EATTR_OP_UNKNOWN;
}

// caller should free result
static bool eclass_attr_setter_op(t_object* x, t_symbol* prop_name, t_eattr_op op, t_float v,
    int& argc,
    t_atom*& argv)
{
    if (op == EATTR_OP_UNKNOWN)
        return false;

    auto z = reinterpret_cast<t_ebox*>(x);

    int new_argc = 0;
    t_atom* new_argv = nullptr;
    // get current values
    auto ok = eclass_attr_getter(x, prop_name, &new_argc, &new_argv);
    if (ok && new_argc && new_argv) {
        // apply math to copy of current values
        // calc arguments
        for (int i = 0; i < new_argc; i++) {
            auto a = &new_argv[i];
            if (atom_gettype(a) != A_FLOAT)
                continue;

            t_float& av = a->a_w.w_float;

            switch (op) {
            case EATTR_OP_ADD:
                av += v;
                break;
            case EATTR_OP_SUB:
                av -= v;
                break;
            case EATTR_OP_MUL:
                av *= v;
                break;
            case EATTR_OP_DIV:
                if (v == 0) {
                    pd_error(x, "[%s][@%s] division by zero",
                        eobj_getclassname(&z->b_obj)->s_name, prop_name->s_name);
                    freebytes(new_argv, new_argc);
                    return false;
                }

                av /= v;
                break;
            default:
                break;
            }
        }

        argc = new_argc;
        argv = new_argv;
        return true;
    } else
        return false;
}

int eclass_attr_setter(t_object* x, t_symbol* s, int argc, t_atom* argv)
{
    t_ebox* z = reinterpret_cast<t_ebox*>(x);
    t_eclass* c = reinterpret_cast<t_eclass*>(z->b_obj.o_obj.te_g.g_pd);

    auto is_float_type = [](const t_symbol* t) { return t == &s_float || t == s_double; };
    auto is_symbol_op = [](const t_atom* a) {
        if (a->a_type != A_SYMBOL)
            return false;
        const auto ch = a->a_w.w_symbol->s_name;

        if (ch[0] == '\0')
            return false;
        if (ch[1] != '\0')
            return false;

        return ch[0] == '+' || ch[0] == '-' || ch[0] == '*' || ch[0] == '/';
    };
    auto is_float_op = [&](t_eattr* attr, int argc, t_atom* argv) {
        return is_float_type(attr->type)
            && argc == 2
            && is_symbol_op(&argv[0])
            && atom_gettype(&argv[1]) == A_FLOAT;
    };
    auto clip_args = [](const t_eattr* attr, int argc, t_atom* argv) {
        if (attr->clipped & E_CLIP_MIN) {
            for (int j = 0; j < argc; j++) {
                if (atom_gettype(argv + j) == A_FLOAT) {
                    atom_setfloat(argv + j, pd_clip_min(atom_getfloat(argv + j), attr->minimum));
                }
            }
        }

        if (attr->clipped & E_CLIP_MAX) {
            for (int j = 0; j < argc; j++) {
                if (atom_gettype(argv + j) == A_FLOAT) {
                    atom_setfloat(argv + j, pd_clip_max(atom_getfloat(argv + j), attr->maximum));
                }
            }
        }
    };
    auto prop_setter = [&](t_eattr* attr, int argc, t_atom* argv) {
        // math operators: +-*/
        // example: + 0.4
        if (is_float_op(attr, argc, argv)) {
            int new_argc = 0;
            t_atom* new_argv = nullptr;

            auto ok = eclass_attr_setter_op(x, s,
                atom2op(&argv[0]), atom_getfloat(&argv[1]),
                new_argc, new_argv);

            if (ok) {
                clip_args(attr, new_argc, new_argv);
                attr->setter(x, attr, new_argc, new_argv);
                freebytes(new_argv, new_argc);
            } else {
                clip_args(attr, argc, argv);
                attr->setter(x, attr, argc, argv);
            }
        } else {
            clip_args(attr, argc, argv);
            attr->setter(x, attr, argc, argv);
        }
    };

    for (size_t i = 0; i < c->c_nattr; i++) {
        size_t size;
        t_eattr* attr = c->c_attr[i];

        if (attr->name != s)
            continue;

        if (attr->sizemax == 0) {
            size = attr->size;
        } else {
            if (argc > attr->sizemax) {
                argc = attr->sizemax;
            }
            size = argc;
        }

        void* point = (char*)(x) + attr->offset;
        const auto type = attr->type;

        if (attr->getter) {
            if (attr->setter) { // getter and setter
                prop_setter(attr, argc, argv);
            } else { // getter only (readonly)
                pd_error(x, "[%s] readonly property: @%s", c->c_class.c_name->s_name, attr->name->s_name);
            }
        } else if (attr->getter == nullptr && attr->setter != nullptr) {
            // setter only (using default set method), getter is default reading
            prop_setter(attr, argc, argv);
        } else if (type == s_int || type == s_long) {
            t_eattr_op op = EATTR_OP_ASSIGN;
            int arg_offset = 0;

            if (atom_gettype(argv) == A_SYMBOL) {
                arg_offset = 1;
                op = atom2op(argv);
            }

            if (op == EATTR_OP_UNKNOWN) {
                const char* op = atom_getsymbol(argv)->s_name;
                pd_error(x,
                    "[%s] unknown operator for property @%s: '%s'",
                    eobj_getclassname(&z->b_obj)->s_name,
                    s->s_name, op);
                return false;
            }

            const size_t N = argc - arg_offset;

            if (N > size) {
                pd_error(x,
                    "[%s] invalid argument count for @%s: %d, max number is %d",
                    eobj_getclassname(&z->b_obj)->s_name,
                    s->s_name, argc, (int)size);
                return false;
            }

            for (size_t j = 0; j < N; j++) {
                const auto a = argv + j + arg_offset;
                if (atom_gettype(a) == A_FLOAT) {
                    ebox_attr_long_setter(z, attr, atom_getlong(a), j, op);
                } else {
                    char buf[32];
                    atom_string(a, buf, sizeof(buf) - 1);
                    pd_error(x, "[%s] not float argument for property @%s: %s, ignoring",
                        eobj_getclassname(&z->b_obj)->s_name,
                        s->s_name,
                        buf);
                }
            }
        } else if (is_float_type(type)) {
            t_eattr_op op = EATTR_OP_ASSIGN;
            int arg_offset = 0;

            if (atom_gettype(argv) == A_SYMBOL) {
                arg_offset = 1;
                op = atom2op(argv);
            }

            if (op == EATTR_OP_UNKNOWN) {
                const char* op_str = atom_getsymbol(argv)->s_name;
                pd_error(x,
                    "[%s] unknown operator for property @%s: '%s'",
                    eobj_getclassname(&z->b_obj)->s_name,
                    s->s_name, op_str);
                return false;
            }

            const size_t N = argc - arg_offset;

            if (N > size) {
                pd_error(x,
                    "[%s] invalid argument count for @%s: %d, max number is %d",
                    eobj_getclassname(&z->b_obj)->s_name,
                    s->s_name, argc, (int)size);
                return false;
            }

            for (size_t j = 0; j < N; j++) {
                const auto a = argv + j + arg_offset;

                if (atom_gettype(a) == A_FLOAT) {
                    if (!ebox_attr_float_setter(z, attr, atom_getfloat(a), j, op))
                        pd_error(z, "can't set property: %s", s->s_name);
                }
            }
        } else if (type == &s_symbol) {
            t_symbol** pointor = (t_symbol**)point;
            for (size_t j = 0; j < size && j < argc; j++) {
                if (atom_gettype(argv + j) == A_SYMBOL) {
                    pointor[j] = gensym(atom_getsymbol(argv + j)->s_name);
                }
            }
        } else if (type == s_atom) {
            clip_args(attr, argc, argv);

            t_atom* pointor = (t_atom*)point;
            for (size_t j = 0; j < size && j < argc; j++) {
                pointor[j] = argv[j];
            }
        }

        // handle @size or @pinned change
        ebox_notify(z, s);

        if (c->c_widget.w_notify != nullptr)
            c->c_widget.w_notify(z, s, s_attr_modified);

        if (attr->paint) {
            if (c->c_widget.w_oksize != nullptr) {
                c->c_widget.w_oksize(z, &z->b_rect);
            }
            if (c->c_widget.w_getdrawparameters != nullptr) {
                c->c_widget.w_getdrawparameters(z, &z->b_boxparameters);
            }

            ebox_redraw(z);
        }

        // mark as changed for gui objects
        if (attr->save && eobj_isbox(&z->b_obj) && ebox_isdrawable(z))
            canvas_dirty(eobj_getcanvas(&z->b_obj), 1);

        return 1;
    }

    pd_error(x, "[%s] property not found: %s", eobj_getclassname(&z->b_obj)->s_name, s->s_name);
    return 0;
}

static void ewidget_init(t_eclass* c)
{
    c->c_widget.w_getrectfn = ebox_wgetrect;
    c->c_widget.w_visfn = ebox_wvis;
    c->c_widget.w_displacefn = ebox_wdisplace;
    c->c_widget.w_selectfn = ebox_wselect;
    c->c_widget.w_activatefn = nullptr;
    c->c_widget.w_deletefn = ebox_wdelete;
    c->c_widget.w_clickfn = nullptr;

    c->c_widget.w_paint = nullptr;
    c->c_widget.w_create = nullptr;
    c->c_widget.w_erase = nullptr;
    c->c_widget.w_mouseenter = nullptr;
    c->c_widget.w_mouseleave = nullptr;
    c->c_widget.w_mousemove = nullptr;
    c->c_widget.w_mousedown = nullptr;
    c->c_widget.w_mousedrag = nullptr;
    c->c_widget.w_mouseup = nullptr;
    c->c_widget.w_mousewheel = nullptr;
    c->c_widget.w_dblclick = nullptr;
    c->c_widget.w_rightclick = nullptr;
    c->c_widget.w_key = nullptr;
    c->c_widget.w_keyfilter = nullptr;
    c->c_widget.w_getdrawparameters = nullptr;
    c->c_widget.w_notify = nullptr;
    c->c_widget.w_save = nullptr;
    c->c_widget.w_dosave = nullptr;
    c->c_widget.w_popup = nullptr;
    c->c_widget.w_dsp = nullptr;
    c->c_widget.w_oksize = nullptr;
    c->c_widget.w_write = nullptr;
    c->c_widget.w_read = nullptr;
    c->c_widget.w_onzoom = nullptr;
}

#define DIALOG_WINDOW_PADX " -padx 8 "
#define DIALOG_WINDOW_PADY " -pady 8 "

static const char* dialog_frame_id(int i)
{
    static char buf[100];
    snprintf(buf, 100, "$fp.frame%i", i);
    return buf;
}

static const char* dialog_label_id(int i)
{
    static char buf[100];
    snprintf(buf, 100, "$fp.l%i", i);
    return buf;
}

static const char* dialog_widget_id(int i)
{
    static char buf[100];
    snprintf(buf, 100, "$fp.w%i", i);
    return buf;
}

//! Initialize the tcl/tk properties dialog window functions // PRIVATE
/*
 \ @memberof            eclass
 \ @param c             The eclass pointer
 \ @return              Nothing
 */
static void eclass_properties_dialog(t_eclass* c)
{
    static const char* proc_color = "proc pdtk_{0}_picker_apply_{1} {{id red green blue alpha}} {{\n"
                                    "   set col [eobj_rgba_to_hex $red $green $blue $alpha]\n"
                                    /// temp fix for bug when scrollbox popup behind the topmost window
                                    /// "   wm attributes $id -topmost 0\n"
                                    "   set color [tk_chooseColor -title {{{0}}} -initialcolor ${{col}} -parent $id ]\n"
                                    /// "   wm attributes $id -topmost 1 \n"
                                    "   if {{$color == \"\"}} return \n"
                                    "   foreach {{red2 green2 blue2}} [winfo rgb . $color] {{}}\n"
                                    "   set cmd [concat $id dialog $id {2} @{1} [eobj_rgb_int_to_float $red2 $green2 $blue2]]\n"
                                    "   pdsend $cmd\n"
                                    "}}\n";

    static const char* proc_entry = "proc pdtk_{0}_dialog_apply_{1} {{id}} {{ \n"
                                    "   set vid [string trimleft $id .]\n"
                                    "   set var_{1} [concat {1}_$vid] \n"
                                    /// replace dollar $N with #N
                                    /// tcl: regsub -all {\$(\d+)} $s {#\1}
                                    "   global $var_{1} \n"
                                    "   set cmd [concat $id dialog $id {2} @{1} "
                                    "       [regsub -all {{\\$(\\d+)}} [eval concat $$var_{1}] {{#\\1}} ]]\n"
                                    "   pdsend $cmd\n"
                                    "}}\n";

    static const char* proc_num = "proc pdtk_{0}_dialog_apply_{1} {{id}} {{ \n"
                                  "   set vid [string trimleft $id .]\n"
                                  "   set var_{1} [concat {1}_$vid] \n"
                                  "   global $var_{1} \n"
                                  "   set cmd [concat $id dialog $id {2} @{1} [eval concat $$var_{1}]]\n"
                                  "   pdsend $cmd\n"
                                  "}}\n";

    static const bool is_ceammc = (getenv("is_ceammc") != nullptr);
    const bool use_sframe = !is_ceammc;

#ifdef __MACH__
    static bool use_theme = false;
    if (!use_theme) {
        use_theme = true;
        if (!is_ceammc)
            sys_gui("ttk::style theme use alt\n");
    }
#endif

    using namespace ceammc;
    const char* class_name = c->c_class.c_name->s_name;

    // DIALOG WINDOW APPLY //
    for (size_t i = 0; i < c->c_nattr; i++) {
        auto attr = c->c_attr[i];
        const char* prop_name = attr->name->s_name;

        switch (crc32_hash(attr->style)) {
        case hash_color:
            sys_gui(fmt::format(proc_color, class_name, prop_name, i + 1).c_str());
            break;
        case hash_number:
        case hash_menu:
        case hash_checkbutton:
            sys_gui(fmt::format(proc_num, class_name, prop_name, i + 1).c_str());
            break;
        default:
            sys_gui(fmt::format(proc_entry, class_name, prop_name, i + 1).c_str());
            break;
        }
    }

    // DIALOG WINDOW CREATION //
    sys_vgui("proc pdtk_%s_dialog {id \n", c->c_class.c_name->s_name);
    for (size_t i = 0; i < c->c_nattr; i++) {
        const char* ATTR_NAME = c->c_attr[i]->name->s_name;
        if (!c->c_attr[i]->invisible) {
            sys_vgui("   %s \n", ATTR_NAME);
        }
    }
    sys_gui("} {\n");
    sys_gui("   set vid [string trimleft $id .]\n");
    if (use_sframe)
        sys_gui("   set fp $id.canvas.container.content\n");
    else
        sys_gui("   set fp $id.f\n");

        /// fill category dict
#ifndef NDEBUG
    sys_gui("   # category dictionary\n");
#endif
    sys_gui("   set cat_dict [dict create]\n");
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->invisible)
            continue;

        auto frame_id = dialog_frame_id(i + 1);
        auto label_id = dialog_label_id(i + 1);
        auto widget_id = dialog_widget_id(i + 1);

        auto str = fmt::format("   dict lappend cat_dict {0} {1}\n"
                               "   dict lappend cat_dict {0} {2}\n"
                               "   dict lappend cat_dict {0} {3}\n",
            c->c_attr[i]->category->s_name,
            frame_id, label_id, widget_id);
        sys_gui(str.c_str());
    }

    /// set global vars
#ifndef NDEBUG
    sys_gui("   # global vars\n");
#endif
    for (size_t i = 0; i < c->c_nattr; i++) {
        const char* ATTR_NAME = c->c_attr[i]->name->s_name;
        if (!c->c_attr[i]->invisible) {
            auto str = fmt::format("   set var_{0} [string trim [concat {0}_$vid]]\n"
                                   "   global $var_{0} \n"
                                   "   set $var_{0} [string trim ${0}]\n",
                ATTR_NAME,
                c->c_attr[i]->category->s_name);
            sys_gui(str.c_str());
        }
    }

    /// window creation
#ifndef NDEBUG
    sys_gui("   # window\n");
#endif
    // _("%s properties")
    //
    if (use_sframe) {
        auto str = fmt::format(
            "   ::ceammc::ui::sframe::new $id -toplevel true -anchor w\n"
            "   wm title $id [format [_ \"%s properties\" ] {{{0}}}] \n"
            "   wm resizable $id 0 1\n" /// resize only height
            "   wm minsize $id 200 300\n"
            "   bind $id <Key-Escape> \"destroy $id; break\"\n" /// close dialog by key-pressing 'Escape'
            "   raise [winfo toplevel $id]\n"
            "   $id configure " DIALOG_WINDOW_PADX DIALOG_WINDOW_PADY "\n"
            "   grid $fp\n",
            c->c_class.c_name->s_name);
        sys_gui(str.c_str());
    } else {
        auto str = fmt::format(
            "   toplevel $id -background $::pd_colors::window_background\n"
            "   wm title $id [format [_ \"%s properties\" ] {{{0}}}] \n"
            "   wm minsize $id 200 300\n"
            "   bind $id <Key-Escape> \"destroy $id; break\"\n" /// close dialog by key-pressing 'Escape'
            "   raise [winfo toplevel $id]\n"
            "   $id configure " DIALOG_WINDOW_PADX DIALOG_WINDOW_PADY "\n"
            "   set fp [ttk::frame $id.f]\n"
            "   grid $fp\n",
            c->c_class.c_name->s_name);
        sys_gui(str.c_str());
    }

    t_symbol* cat = &s_;
    int category_idx = 0;
    const std::array<t_symbol*, 2> hidden_by_default_cat { gensym("Label"), gensym("Colors") };
    for (size_t i = 0; i < c->c_nattr; i++) {
        auto attr = c->c_attr[i];

        if (!attr->invisible) {
            const char* FRAME_ID = dialog_frame_id(i + 1);
            const char* LABEL_ID = dialog_label_id(i + 1);
            const char* WIDGET_ID = dialog_widget_id(i + 1);
            const char* ATTR_NAME = attr->name->s_name;
            const char* CLASS_NAME = c->c_class.c_name->s_name;

            /** PROPERTY CATEGORY **/
            if (attr->category != cat) {
                bool show_first_time = (std::find(hidden_by_default_cat.begin(),
                                            hidden_by_default_cat.end(),
                                            attr->category)
                    == hidden_by_default_cat.end());
                auto str = fmt::format(
                    "   global var_cat{0}_state\n"
                    "   if {{ [info exists var_cat{0}_state] eq 0 }} {{ set var_cat{0}_state {2} }}\n"
                    "   ttk::label $fp.cat_img{0} -image [ceammc_category_icon var_cat{0}_state]\n"
                    "   bind $fp.cat_img{0} <Button> [list ceammc_category_toggle"
                    "       $fp.cat_img{0} var_cat{0}_state [concat [dict get $cat_dict \"{0}\"]]]\n"
                    "   ttk::label $fp.cat_lbl{0} -justify left -text [_ \"{0}\"] -font CICMCategoryFont\n"
                    "   grid config $fp.cat_img{0} -column 0 -row {1} -sticky w\n"
                    "   grid config $fp.cat_lbl{0} -column 1 -columnspan 2 -row {1} -sticky nwse\n",
                    attr->category->s_name, i + category_idx + 1, (int)show_first_time);
                // update current category
                cat = attr->category;
                category_idx++;
                sys_gui(str.c_str());
            }

            /** ATTRIBUTES NAMES **/
            auto str = fmt::format("   # property: @{2}\n"
                                   "   ttk::label {0} -justify left -text [join [list [_ \"{1}\" ] {{:}}] {{}}]\n"
                                   "   ceammc_tooltip {0} \"@{2}\"\n",
                LABEL_ID, attr->label->s_name, ATTR_NAME);
            sys_gui(str.c_str());

            /** SELECTOR WIDGETS **/
            switch (crc32_hash(attr->style)) {
            case hash_checkbutton: {
                auto str = fmt::format(
                    "   ttk::checkbutton {0} -variable [string trim $var_{2}] "
                    "   -command [concat pdtk_{1}_dialog_apply_{2} $id]\n",
                    WIDGET_ID, CLASS_NAME, ATTR_NAME);
                sys_gui(str.c_str());
            } break;
            case hash_color: {
                auto str = fmt::format(
                    "   set color [eval eobj_rgba_to_hex ${2}]\n"
                    "   entry {0} -font {{Helvetica 11}} -width 10 -readonlybackground $color -state readonly\n"
                    "   bind  {0} <Button> [concat pdtk_{1}_picker_apply_{2} $id ${2}]\n",
                    WIDGET_ID, CLASS_NAME, ATTR_NAME);
                sys_gui(str.c_str());
            } break;
            case hash_number: {
                auto str = fmt::format(
                    "   ttk::spinbox {0} -width 18 -textvariable [string trim $var_{2}] -increment {3} \n"
                    "   {0} configure -command [concat pdtk_{1}_dialog_apply_{2} $id]\n"
                    "   {0} configure -from -9999999999999 -to 9999999999999\n"
                    "   ::ceammc::ui::bindMouseWheel {0} {{ ::ceammc::ui::spinboxScroll %W }}\n"
                    "   {0} delete 0 end \n"
                    "   {0} insert 0 ${2} \n"
                    "   bind {0} <KeyPress-Return> [concat pdtk_{1}_dialog_apply_{2} $id]\n",
                    WIDGET_ID, CLASS_NAME, ATTR_NAME, (float)c->c_attr[i]->step);

                sys_gui(str.c_str());
            } break;
            case hash_menu: {
                std::string tmpl(
                    "   ttk::combobox {0} -width 16 -state readonly -textvariable [string trim $var_{2}]\n"
                    "   {0} configure -values {{ ");

                // append values
                for (size_t j = 0; j < c->c_attr[i]->itemssize; j++) {
                    tmpl += c->c_attr[i]->itemslist[c->c_attr[i]->itemssize - 1 - j]->s_name;
                    tmpl.push_back(' ');
                }

                tmpl += "}}\n"
                        "   bind {0} <<ComboboxSelected>> [concat pdtk_{1}_dialog_apply_{2} $id]\n"
                        "   ::ceammc::ui::bindMouseWheel {0} {{ ::ceammc::ui::comboboxScroll %W }}\n"
                        "   {0} set [string trim ${2}] \n";

                sys_gui(fmt::format(tmpl, WIDGET_ID, CLASS_NAME, ATTR_NAME).c_str());
            } break;
            case hash_path: {
                auto str = fmt::format(
                    "   ttk::entry {0} -width 20 -textvariable [string trim $var_{2}]\n"
                    "   bind {0} <KeyPress-Return> [concat pdtk_{1}_dialog_apply_{2} $id]\n"
                    "   proc cicm_dialog_{1}_open_{2} {varname id} {\n"
                    "      global $varname\n"
                    "      set types {{ {{Image Files}} {{.gif .png .jpg .jpeg}} }}\n"
                    "      set $varname [tk_getOpenFile -initialfile $$varname -filetypes $types]\n"
                    "      pdtk_{1}_dialog_apply_{2} $id\n"
                    "   }\n"
                    "   ttk::button {0}_{2}_file -text [_ \"{3}\"] -command [concat cicm_dialog_{1}_open_{2} [string trim $var_{2}] $id]\n",
                    WIDGET_ID, CLASS_NAME, ATTR_NAME, _("Select"));
                sys_gui(str.c_str());
            }
            default: {
                str = fmt::format(
                    "   ttk::entry {0} -width 20 -textvariable [string trim $var_{1}]\n"
                    // erase (null) on focus in
                    // insert (null) on focus out
                    "   bind {0} <FocusIn> {{ if {{ [string trim [%W get]] == {{(null)}} }} {{ %W delete 0 end }} }}\n"
                    "   bind {0} <FocusOut> {{ if {{ [string trim [%W get]] == {{}} }} {{ %W insert 0 {{(null)}} }} }}\n"
                    "   bind {0} <KeyPress-Return> {{ if {{ [string trim [%W get]] == {{}} }} {{ %W insert 0 {{(null)}} }} }}\n"
                    "   bind {0} <KeyPress-Return> +[concat pdtk_{2}_dialog_apply_{1} $id]\n",
                    WIDGET_ID, ATTR_NAME, CLASS_NAME);
                sys_gui(str.c_str());
            } break;
            }

            str = fmt::format(
                "   grid [ttk::frame {1}] -column 0 -row {0} -sticky nwse\n"
                "   grid {2} -column 1 -row {0} -sticky nw\n"
                "   grid {3} -column 2 -row {0} -sticky nwse\n",
                i + category_idx + 1, FRAME_ID, LABEL_ID, WIDGET_ID);
            sys_gui(str.c_str());
        }
    }

    {
        // show/hide categories
        t_symbol* cat = &s_;
        for (size_t i = 0; i < c->c_nattr; i++) {
            auto attr = c->c_attr[i];
            if (attr->invisible)
                continue;

            if (cat == attr->category)
                continue;

            cat = attr->category;
            auto str = fmt::format(
                "   if {{ $var_cat{0}_state eq 0 }} {{\n"
                "      set lst [dict get $cat_dict \"{0}\"]\n"
                "      ceammc_category_apply $fp.cat_img{0} var_cat{0}_state $lst\n"
                "   }}\n",
                cat->s_name);
            sys_gui(str.c_str());
        }
    }

    sys_gui("}\n");
}

static int attr_cmp(const void* p1, const void* p2)
{
    const t_eattr* a1 = *(const t_eattr**)p1;
    const t_eattr* a2 = *(const t_eattr**)p2;

    if (a1->order == a2->order)
        return 0;
    else if (a1->order < a2->order)
        return -1;
    else
        return 1;
}

void eclass_attr_sort(t_eclass* c)
{
    qsort(c->c_attr, c->c_nattr, sizeof(t_eattr*), attr_cmp);
}

void eclass_attr_visible(t_eclass* c, const char* attrname)
{
    auto sel = gensym(attrname);
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            c->c_attr[i]->invisible = false;
            return;
        }
    }
}

void eclass_attr_units(t_eclass* c, t_symbol* attrname, t_symbol* units)
{
    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == attrname) {
            c->c_attr[i]->units = units;
            return;
        }
    }
}
