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
#include "ebox.h"
#include "ecommon.h"
#include "egraphics.h"
#include "eobj.h"
#include "epopup.h"
#include "fmtmsg.h"

#include "../ceammc/extra/fmt/include/fmt/core.h"
#include "../ceammc/extra/fmt/include/fmt/format.h"

#include <iostream>
#include <vector>

#define _(msg) msg

static const char* SYM_DUMP = "dump";
static const char* SYM_GET_ALL_PROPS = "@*?";
static const char* SYM_DIALOG = "dialog";

static const char* SYM_MOUSE_ENTER = "mouseenter";
static const char* SYM_MOUSE_LEAVE = "mouseleave";
static const char* SYM_MOUSE_MOVE = "mousemove";
static const char* SYM_MOUSE_DOWN = "mousedown";
static const char* SYM_MOUSE_DRAG = "mousedrag";
static const char* SYM_MOUSE_UP = "mouseup";
static const char* SYM_MOUSE_WHEEL = "mousewheel";
static const char* SYM_MOUSE_DBL_CLICK = "dblclick";
static const char* SYM_KEY = "key";
static const char* SYM_KEY_FILTER = "keyfilter";

static const char* SYM_READ = "read";
static const char* SYM_WRITE = "write";

static const char* SYM_PAINT = "paint";
static const char* SYM_NOTIFY = "notify";
static const char* SYM_GET_DRAW_PARAMS = "getdrawparams";
static const char* SYM_OK_SIZE = "oksize";
static const char* SYM_ONZOOM = "onzoom";
static const char* SYM_SAVE = "save";
static const char* SYM_POPUP = "popup";
static const char* SYM_PRESET = "preset";

static const char* SYM_POS = "pos";
static const char* SYM_VIS = "vis";
static const char* SYM_ZOOM = "zoom";

static const char* SYM_ENTRY = "entry";
static const char* SYM_CHECKBUTTON = "checkbutton";
static const char* SYM_ON_OFF = "onoff";
static const char* SYM_MENU = "menu";
static const char* SYM_COLOR = "color";
static const char* SYM_RGB = "rgb";
static const char* SYM_RGBA = "rgba";
static const char* SYM_PATH = "path";
static const char* SYM_NUMBER = "number";
static const char* SYM_SPINBOX = "spinbox";

static const char* SYM_ANY = "anything";
static const char* SYM_DSP = "dsp";
static const char* SYM_DSP_ADD = "dsp_add";
static const char* SYM_DSP_ADD64 = "dsp_add";

enum CategoryType {
    CAT_BASE = 0,
    CAT_COLOR = 100,
    CAT_LABEL = 200,
    CAT_BOUNDS = 300,
    CAT_MAIN = 400,
    CAT_MIDI = 500,
    CAT_MISC = 600
};

static void eclass_properties_dialog(t_eclass* c);
static void ewidget_init(t_eclass* c);
static void eclass_attr_ceammc_setter(t_object* x, t_symbol* s, int argc, t_atom* argv);

static t_class* tcl_version_class = 0;
typedef struct tcl_version {
    t_object x_obj;
    char minor;
    char major;
} t_tcl_version;

t_tcl_version* tcl_version_instance;
t_tcl_version* tcl_version_new()
{
    t_tcl_version* x = (t_tcl_version*)pd_new(tcl_version_class);
    if (x) {
        x->minor = 0;
        x->major = 0;
    }

    return x;
}

int egraphics_smooth()
{
    if (!tcl_version_class || !tcl_version_instance)
        return 0;

    if (tcl_version_instance->major == 8 && tcl_version_instance->minor == 6)
        return 1;
    else
        return 0;
}

void tcl_version_set(t_tcl_version* x, t_float v)
{
    float int_part;
    float frac_part = modff(v, &int_part);

    x->major = (char)int_part;
    x->minor = (char)roundf(frac_part * 10);

    if (x->minor != 6)
        post("[ceammc] TCL/Tk version is: %d.%d", x->major, x->minor);
}

void tcl_version_init()
{
    if (tcl_version_class == 0) {
        tcl_version_class = class_new(gensym("_tcl.version"),
            (t_newmethod)tcl_version_new, 0,
            sizeof(t_tcl_version), 0, A_NULL);

        class_addmethod(tcl_version_class, (t_method)&tcl_version_set,
            gensym("tcl_version"), A_DEFFLOAT, A_NULL);
        pd_bind(&tcl_version_class, gensym("tcl_version"));
        tcl_version_instance = tcl_version_new();
        sys_gui("pdsend \"tcl_version tcl_version $tk_version\"\n");
    }
}

t_eclass* eclass_new(const char* name, t_typ_method newm, t_typ_method freem, size_t size, int flags, t_atomtype arg1, int arg2)
{
    t_class* pd = class_new(gensym(name), (t_newmethod)newm, (t_method)freem, size, flags, arg1, arg2);
    if (!pd) {
        bug("PureData Memory allocation failed for the class %s.", name);
        return 0;
    }

    // we are using same size to prevent PD from memset to 0 resized memory.
    // we should do this way because some versions of PD have different sizeof(t_class)
    t_eclass* c = (t_eclass*)resizebytes(pd, sizeof(t_eclass), sizeof(t_eclass));

    if (c) {
        tcl_version_init();
        epd_init();
        memset(&c->c_widget, 0, sizeof(t_ewidget));
        c->c_attr = 0;
        c->c_nattr = 0;
        c->c_dsp = 0;
        c->c_box = 0;
        c->c_attr = NULL;
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

void eclass_guiinit(t_eclass* c, long flags)
{
    ewidget_init(c);
    c->c_box = 1;

    // DEFAULT ATTRIBUTES //
    CLASS_ATTR_FLOAT_ARRAY(c, "size", 0, t_ebox, b_rect.width, 2);
    CLASS_ATTR_SYMBOL(c, "fontname", 0, t_ebox, b_font.c_family);
    CLASS_ATTR_SYMBOL(c, "fontweight", 0, t_ebox, b_font.c_weight);
    CLASS_ATTR_SYMBOL(c, "fontslant", 0, t_ebox, b_font.c_slant);
    CLASS_ATTR_INT(c, "fontsize", 0, t_ebox, b_font.c_sizereal);
    CLASS_ATTR_SYMBOL(c, "receive", 0, t_ebox, b_receive_id);
    CLASS_ATTR_SYMBOL(c, "send", 0, t_ebox, b_send_id);
    CLASS_ATTR_SYMBOL(c, "label", 0, t_ebox, b_label);
    CLASS_ATTR_INT(c, "label_inner", 0, t_ebox, label_inner);
    CLASS_ATTR_SYMBOL(c, "label_side", 0, t_ebox, label_side);
    CLASS_ATTR_SYMBOL(c, "label_align", 0, t_ebox, label_align);
    CLASS_ATTR_SYMBOL(c, "label_valign", 0, t_ebox, label_valign);
    CLASS_ATTR_INT_ARRAY(c, "label_margins", 0, t_ebox, label_margins, 2);

    CLASS_ATTR_DEFAULT(c, "size", 0, "100. 100.");
    CLASS_ATTR_FILTER_MIN(c, "size", 4);
    CLASS_ATTR_SAVE(c, "size", 0);
    CLASS_ATTR_PAINT(c, "size", 0);
    CLASS_ATTR_CATEGORY(c, "size", 0, _("Basic"));
    CLASS_ATTR_LABEL(c, "size", 0, _("Patching Size"));
    CLASS_ATTR_ACCESSORS(c, "size", NULL, (t_err_method)ebox_size_set);

    CLASS_ATTR_INT(c, "pinned", 0, t_ebox, b_pinned);
    CLASS_ATTR_DEFAULT(c, "pinned", 0, "0");
    CLASS_ATTR_FILTER_CLIP(c, "pinned", 0, 1);
    CLASS_ATTR_SAVE(c, "pinned", 0);
    CLASS_ATTR_CATEGORY(c, "pinned", 0, _("Basic"));
    CLASS_ATTR_LABEL(c, "pinned", 0, _("Pinned"));
    CLASS_ATTR_STYLE(c, "pinned", 0, "onoff");

    CLASS_ATTR_DEFAULT(c, "fontname", 0, "Helvetica");
    CLASS_ATTR_SAVE(c, "fontname", 0);
    CLASS_ATTR_PAINT(c, "fontname", 0);
    CLASS_ATTR_CATEGORY(c, "fontname", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "fontname", 0, _("Font Name"));
    CLASS_ATTR_ACCESSORS(c, "fontname", NULL, (t_err_method)ebox_set_font);
    CLASS_ATTR_STYLE(c, "fontname", 0, "menu");
    CLASS_ATTR_ITEMS(c, "fontname", 0, "Helvetica Monaco Courier Times DejaVu");

    CLASS_ATTR_DEFAULT(c, "fontweight", 0, _("normal"));
    CLASS_ATTR_SAVE(c, "fontweight", 0);
    CLASS_ATTR_PAINT(c, "fontweight", 0);
    CLASS_ATTR_CATEGORY(c, "fontweight", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "fontweight", 0, _("Font Weight"));
    CLASS_ATTR_ACCESSORS(c, "fontweight", NULL, (t_err_method)ebox_set_fontweight);
    CLASS_ATTR_STYLE(c, "fontweight", 0, "menu");
    CLASS_ATTR_ITEMS(c, "fontweight", 0, "normal bold");

    CLASS_ATTR_DEFAULT(c, "fontslant", 0, "roman");
    CLASS_ATTR_SAVE(c, "fontslant", 0);
    CLASS_ATTR_PAINT(c, "fontslant", 0);
    CLASS_ATTR_CATEGORY(c, "fontslant", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "fontslant", 0, _("Font Slant"));
    CLASS_ATTR_ACCESSORS(c, "fontslant", NULL, (t_err_method)ebox_set_fontslant);
    CLASS_ATTR_STYLE(c, "fontslant", 0, "menu");
    CLASS_ATTR_ITEMS(c, "fontslant", 0, "roman italic");

    CLASS_ATTR_DEFAULT(c, "fontsize", 0, "11");
    CLASS_ATTR_FILTER_MIN(c, "fontsize", 4);
    CLASS_ATTR_SAVE(c, "fontsize", 0);
    CLASS_ATTR_PAINT(c, "fontsize", 0);
    CLASS_ATTR_CATEGORY(c, "fontsize", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "fontsize", 0, _("Font Size"));
    CLASS_ATTR_ACCESSORS(c, "fontsize", NULL, ebox_set_fontsize);
    CLASS_ATTR_STYLE(c, "fontsize", 0, "number");

    CLASS_ATTR_DEFAULT(c, "receive", 0, "(null)");
    CLASS_ATTR_ACCESSORS(c, "receive", NULL, ebox_set_receiveid);
    CLASS_ATTR_SAVE(c, "receive", 0);
    CLASS_ATTR_CATEGORY(c, "receive", 0, _("Basic"));
    CLASS_ATTR_LABEL(c, "receive", 0, _("Receive Symbol"));

    CLASS_ATTR_DEFAULT(c, "send", 0, "(null)");
    CLASS_ATTR_ACCESSORS(c, "send", NULL, ebox_set_sendid);
    CLASS_ATTR_SAVE(c, "send", 0);
    CLASS_ATTR_CATEGORY(c, "send", 0, _("Basic"));
    CLASS_ATTR_LABEL(c, "send", 0, _("Send Symbol"));

    CLASS_ATTR_DEFAULT(c, "label", 0, "(null)");
    CLASS_ATTR_ACCESSORS(c, "label", NULL, ebox_set_label);
    CLASS_ATTR_SAVE(c, "label", 0);
    CLASS_ATTR_CATEGORY(c, "label", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "label", 0, _("Text"));

    CLASS_ATTR_DEFAULT(c, "label_align", 0, "left");
    CLASS_ATTR_ACCESSORS(c, "label_align", NULL, ebox_set_label_align);
    CLASS_ATTR_SAVE(c, "label_align", 0);
    CLASS_ATTR_CATEGORY(c, "label_align", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "label_align", 0, _("Horizontal align"));
    CLASS_ATTR_STYLE(c, "label_align", 0, "menu");
    CLASS_ATTR_ITEMS(c, "label_align", 0, "left center right");

    CLASS_ATTR_DEFAULT(c, "label_valign", 0, "top");
    CLASS_ATTR_ACCESSORS(c, "label_valign", NULL, ebox_set_label_valign);
    CLASS_ATTR_SAVE(c, "label_valign", 0);
    CLASS_ATTR_CATEGORY(c, "label_valign", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "label_valign", 0, _("Vertical align"));
    CLASS_ATTR_STYLE(c, "label_valign", 0, "menu");
    CLASS_ATTR_ITEMS(c, "label_valign", 0, "top center bottom");

    CLASS_ATTR_DEFAULT(c, "label_side", 0, "top");
    CLASS_ATTR_ACCESSORS(c, "label_side", NULL, ebox_set_label_side);
    CLASS_ATTR_SAVE(c, "label_side", 0);
    CLASS_ATTR_CATEGORY(c, "label_side", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "label_side", 0, _("Attach side"));
    CLASS_ATTR_STYLE(c, "label_side", 0, "menu");
    CLASS_ATTR_ITEMS(c, "label_side", 0, "left top right bottom");

    CLASS_ATTR_DEFAULT(c, "label_inner", 0, "0");
    CLASS_ATTR_ACCESSORS(c, "label_inner", NULL, ebox_set_label_position);
    CLASS_ATTR_SAVE(c, "label_inner", 0);
    CLASS_ATTR_CATEGORY(c, "label_inner", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "label_inner", 0, _("Inner position"));
    CLASS_ATTR_STYLE(c, "label_inner", 0, "onoff");
    CLASS_ATTR_FILTER_CLIP(c, "label_inner", 0, 1);

    CLASS_ATTR_DEFAULT(c, "label_margins", 0, "0 0");
    CLASS_ATTR_SAVE(c, "label_margins", 0);
    CLASS_ATTR_PAINT(c, "label_margins", 0);
    CLASS_ATTR_CATEGORY(c, "label_margins", 0, _("Label"));
    CLASS_ATTR_LABEL(c, "label_margins", 0, _("Margins"));
    CLASS_ATTR_ACCESSORS(c, "label_margins", NULL, ebox_set_label_margins);

    // GUI always need this methods //
    class_addmethod((t_class*)c, (t_method)ebox_attr_dump, gensym(SYM_DUMP), A_NULL, 0);
    class_addmethod((t_class*)c, (t_method)ebox_output_all_attrs, gensym(SYM_GET_ALL_PROPS), A_NULL, 0);
    class_addmethod((t_class*)c, (t_method)ebox_dialog, gensym(SYM_DIALOG), A_GIMME, 0);

    class_addmethod((t_class*)c, (t_method)ebox_mouse_enter, gensym(SYM_MOUSE_ENTER), A_NULL, 0);
    class_addmethod((t_class*)c, (t_method)ebox_mouse_leave, gensym(SYM_MOUSE_LEAVE), A_NULL, 0);
    class_addmethod((t_class*)c, (t_method)ebox_mouse_move, gensym(SYM_MOUSE_MOVE), A_GIMME, 0);
    class_addmethod((t_class*)c, (t_method)ebox_mouse_down, gensym(SYM_MOUSE_DOWN), A_GIMME, 0);
    class_addmethod((t_class*)c, (t_method)ebox_mouse_up, gensym(SYM_MOUSE_UP), A_GIMME, 0);

    class_addmethod((t_class*)c, (t_method)ebox_pos, gensym(SYM_POS), A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod((t_class*)c, (t_method)ebox_vis, gensym(SYM_VIS), A_DEFFLOAT, 0);
    class_addmethod((t_class*)c, (t_method)ebox_setzoom, gensym(SYM_ZOOM), A_DEFFLOAT, 0);

    class_setwidget((t_class*)&c->c_class, (t_widgetbehavior*)&c->c_widget);
    class_setsavefn((t_class*)&c->c_class, (t_savefn)eobj_save);
}

void eclass_dspinit(t_eclass* c)
{
    c->c_dsp = 1;
    class_addmethod((t_class*)c, (t_method)eobj_dsp, gensym(SYM_DSP), A_CANT, 0);
    class_addmethod((t_class*)c, (t_method)eobj_dsp_add, gensym(SYM_DSP_ADD), A_NULL, 0);
    class_addmethod((t_class*)c, (t_method)eobj_dsp_add, gensym(SYM_DSP_ADD64), A_NULL, 0);
}

static t_pd_err is_cicm(t_eobj* x)
{
    return 1;
}

t_pd_err eclass_register(t_symbol* name, t_eclass* c)
{
    if (c->c_dsp) {
        long diff = 0;

        if (c->c_box)
            diff = calcoffset(t_edspbox, d_dsp.d_float);
        else
            diff = calcoffset(t_edspobj, d_dsp.d_float);

        class_domainsignalin(&c->c_class, diff);
    }

    if (c->c_nattr) {
        eclass_properties_dialog(c);
        class_setpropertiesfn((t_class*)c, (t_propertiesfn)ebox_properties);
    }

    class_addmethod((t_class*)c, (t_method)is_cicm, s_iscicm, A_NULL, 0);

    return 0;
}

void eclass_addmethod(t_eclass* c, t_typ_method m, const char* name, t_atomtype type, long dummy)
{
    t_symbol* sname = gensym(name);
    t_class* cx = &c->c_class;
    if (sname == gensym(SYM_MOUSE_ENTER)) {
        c->c_widget.w_mouseenter = m;
    } else if (sname == gensym(SYM_MOUSE_LEAVE)) {
        c->c_widget.w_mouseleave = m;
    } else if (sname == gensym(SYM_MOUSE_MOVE)) {
        c->c_widget.w_mousemove = m;
    } else if (sname == gensym(SYM_MOUSE_DOWN)) {
        c->c_widget.w_mousedown = m;
    } else if (sname == gensym(SYM_MOUSE_DRAG)) {
        c->c_widget.w_mousedrag = m;
    } else if (sname == gensym(SYM_MOUSE_UP)) {
        c->c_widget.w_mouseup = m;
    } else if (sname == gensym(SYM_MOUSE_WHEEL)) {
        class_addmethod(cx, (t_method)ebox_mouse_wheel, gensym(SYM_MOUSE_WHEEL), A_GIMME, 0);
        c->c_widget.w_mousewheel = m;
    } else if (sname == gensym(SYM_MOUSE_DBL_CLICK)) {
        class_addmethod(cx, (t_method)ebox_mouse_dblclick, gensym(SYM_MOUSE_DBL_CLICK), A_GIMME, 0);
        c->c_widget.w_dblclick = m;
    } else if (sname == gensym(SYM_KEY) || sname == gensym(SYM_KEY_FILTER)) {
        if (c->c_widget.w_key == NULL && c->c_widget.w_keyfilter == NULL)
            class_addmethod(cx, (t_method)ebox_key, gensym(SYM_KEY), A_GIMME, 0);
        if (sname == gensym(SYM_KEY))
            c->c_widget.w_key = m;
        if (sname == gensym(SYM_KEY_FILTER))
            c->c_widget.w_keyfilter = m;
    } else if (sname == gensym(SYM_PAINT)) {
        c->c_widget.w_paint = m;
    } else if (sname == gensym(SYM_NOTIFY)) {
        c->c_widget.w_notify = (t_err_method)m;
    } else if (sname == gensym(SYM_GET_DRAW_PARAMS)) {
        c->c_widget.w_getdrawparameters = m;
    } else if (sname == gensym(SYM_OK_SIZE)) {
        c->c_widget.w_oksize = m;
    } else if (sname == gensym(SYM_ONZOOM)) {
        c->c_widget.w_onzoom = m;
    } else if (sname == gensym(SYM_SAVE)) {
        c->c_widget.w_save = m;
    } else if (sname == gensym(SYM_POPUP)) {
        class_addmethod(cx, (t_method)eobj_popup, gensym(SYM_POPUP), A_SYMBOL, A_DEFFLOAT, 0);
        c->c_widget.w_popup = m;
    } else if (sname == gensym(SYM_DSP)) {
        c->c_widget.w_dsp = m;
    } else if (sname == &s_bang) {
        class_addbang(cx, m);
    } else if (sname == &s_float) {
        class_addfloat(cx, m);
    } else if (sname == &s_list) {
        class_addlist(cx, m);
    } else if (sname == gensym(SYM_ANY)) {
        class_addanything(cx, m);
    } else if (sname == &s_symbol) {
        class_addsymbol(cx, m);
    } else if (sname == gensym(SYM_PRESET)) {
        CLASS_ATTR_SYMBOL(c, "presetname", 0, t_ebox, b_objpreset_id);
        CLASS_ATTR_DEFAULT(c, "presetname", 0, "(null)");
        CLASS_ATTR_SAVE(c, "presetname", 0);
        CLASS_ATTR_CATEGORY(c, "presetname", 0, _("Basic"));
        CLASS_ATTR_LABEL(c, "presetname", 0, _("Preset Name"));
        CLASS_ATTR_ACCESSORS(c, "presetname", NULL, ebox_set_presetid);
        class_addmethod(cx, (t_method)m, sname, type, 0);
    } else {
        class_addmethod(cx, (t_method)m, sname, type, 0);
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

void eclass_new_attr_typed(t_eclass* c, const char* attrname, const char* type, long size, long maxsize, long flags, long offset)
{
    int i;
    t_eattr* attr;
    t_eattr** attrs;
    if (size >= 1) {
        t_symbol* name = gensym(attrname);

        for (i = 0; i < c->c_nattr; i++) {
            if (c->c_attr[i]->name == name) {
                error("%s already have %s attribute.", c->c_class.c_name->s_name, attrname);
                return;
            }
        }

        attr = (t_eattr*)getbytes(sizeof(t_eattr));

        if (attr) {
            attr->name = name;
            attr->type = gentype(type);
            attr->category = c->c_class.c_name;
            attr->label = gensym("");
            attr->style = gensym(SYM_ENTRY);
            attr->order = c->c_nattr + 1;
            attr->save = 0;
            attr->paint = 0;
            attr->invisible = 0;
            attr->flags = flags;
            attr->offset = offset;
            attr->size = size;
            attr->sizemax = maxsize;
            attr->getter = NULL;
            attr->setter = NULL;
            attr->clipped = 0;
            attr->minimum = 0;
            attr->maximum = 1;
            attr->step = 1;
            attr->defvals = NULL;
            attr->itemslist = NULL;
            attr->itemssize = 0;

            size_t new_sz = (size_t)(c->c_nattr + 1) * sizeof(t_eattr*);
            attrs = (t_eattr**)resizebytes(c->c_attr, new_sz, new_sz);

            if (attrs) {
                char buf[MAXPDSTRING];
                c->c_attr = attrs;
                c->c_attr[c->c_nattr] = attr;
                sprintf(buf, "@%s", attrname);
                class_addmethod((t_class*)c, (t_method)eclass_attr_ceammc_setter, gensym(buf), A_GIMME, 0);
                sprintf(buf, "@%s?", attrname);
                class_addmethod((t_class*)c, (t_method)eclass_attr_ceammc_getter, gensym(buf), A_GIMME, 0);
                c->c_nattr++;
            } else {
                error("%s can't increase memory for %s attribute.", c->c_class.c_name->s_name, attrname);
            }

        } else {
            error("%s can't allocate memory for %s attribute.", c->c_class.c_name->s_name, attrname);
        }
    } else {
        error("%s %s attribute size is too null.", c->c_class.c_name->s_name, attrname);
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
        t_methodentry* m = &c->c_class.c_methods[i];
        if (m->me_name == sel0 || m->me_name == sel1)
            m->me_fun = fn;
    }
}

void eclass_attr_default(t_eclass* c, const char* attrname, long flags, const char* value)
{
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == gensym(attrname)) {
            c->c_attr[i]->defvals = gensym(value);
            return;
        }
    }

    pd_error(nullptr, "[%s] property not found: %s", c->c_class.c_name->s_name, attrname);
}

void eclass_attr_category(t_eclass* c, const char* attrname, long flags, const char* category)
{
    t_symbol* cat = gensym(category);
    t_symbol* sel = gensym(attrname);

    for (int i = 0; i < c->c_nattr; i++) {
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

void eclass_attr_order(t_eclass* c, const char* attrname, long flags, const char* order)
{
    t_symbol* sel = gensym(attrname);

    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            int n = atoi(order);
            if (isdigit(n))
                c->c_attr[i]->order = n;

            return;
        }
    }
}

void eclass_attr_label(t_eclass* c, const char* attrname, long flags, const char* label)
{
    t_symbol* sel = gensym(attrname);

    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            c->c_attr[i]->label = gensym(label);
            return;
        }
    }
}

void eclass_attr_style(t_eclass* c, const char* attrname, long flags, const char* style)
{
    t_symbol* sel = gensym(attrname);
    t_symbol* s_style = gensym(style);

    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == sel) {
            if (s_style == gensym(SYM_CHECKBUTTON) || s_style == gensym(SYM_ON_OFF)) {
                c->c_attr[i]->style = gensym(SYM_CHECKBUTTON);
            } else if (s_style == gensym(SYM_RGB) || s_style == gensym(SYM_RGBA) || s_style == gensym(SYM_COLOR)) {
                c->c_attr[i]->style = gensym(SYM_COLOR);
            } else if (s_style == gensym(SYM_SPINBOX) || s_style == gensym(SYM_NUMBER)) {
                c->c_attr[i]->style = gensym(SYM_NUMBER);
            } else if (s_style == gensym(SYM_MENU)) {
                c->c_attr[i]->style = gensym(SYM_MENU);
            } else if (s_style == gensym(SYM_PATH)) {
                c->c_attr[i]->style = gensym(SYM_PATH);
            } else {
                c->c_attr[i]->style = gensym(SYM_ENTRY);
            }
            return;
        }
    }
}

void eclass_attr_itemlist(t_eclass* c, const char* attrname, long flags, const char* list)
{
    int i, j = 0;
    char* pch;
    int size = 0;
    t_symbol* s_attrname = gensym(attrname);
    for (i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            auto plist = strdup(list);
            if (!plist)
                return;

            pch = strtok(plist, " ,");

            while (pch != NULL) {
                pch = strtok(NULL, " ,");
                size++;
            }

            free(plist);
            if (size > 0) {
                if (c->c_attr[i]->itemssize) {
                    size_t new_sz = (unsigned long)size * sizeof(t_symbol*);
                    c->c_attr[i]->itemslist = (t_symbol**)resizebytes(c->c_attr[i]->itemslist, new_sz, new_sz);
                    if (c->c_attr[i]->itemslist)
                        c->c_attr[i]->itemssize = size;
                } else {
                    c->c_attr[i]->itemslist = (t_symbol**)getbytes((unsigned long)size * sizeof(t_symbol*));
                    if (c->c_attr[i]->itemslist)
                        c->c_attr[i]->itemssize = size;
                }
                if (c->c_attr[i]->itemslist && c->c_attr[i]->itemssize) {
                    auto plist = strdup(list);
                    if (!plist)
                        return;

                    pch = strtok(plist, " ,");
                    while (pch != NULL && (long)j < c->c_attr[i]->itemssize) {
                        c->c_attr[i]->itemslist[j] = gensym(pch);
                        pch = strtok(NULL, " ,");
                        j++;
                    }

                    free(plist);
                }

            } else {
                if (c->c_attr[i]->itemssize)
                    freebytes(c->c_attr[i]->itemslist, 0);
                c->c_attr[i]->itemssize = 0;
            }
            return;
        }
    }
}

void eclass_attr_filter_min(t_eclass* c, const char* attrname, float value)
{
    t_symbol* s_attrname = gensym(attrname);
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            if (c->c_attr[i]->clipped == 0)
                c->c_attr[i]->clipped = 1;
            else if (c->c_attr[i]->clipped == 2)
                c->c_attr[i]->clipped = 3;

            c->c_attr[i]->minimum = value;
            return;
        }
    }
}

void eclass_attr_filter_max(t_eclass* c, const char* attrname, float value)
{
    t_symbol* s_attrname = gensym(attrname);
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            if (c->c_attr[i]->clipped == 0)
                c->c_attr[i]->clipped = 2;
            else if (c->c_attr[i]->clipped == 1)
                c->c_attr[i]->clipped = 3;

            c->c_attr[i]->maximum = value;
            return;
        }
    }
}

void eclass_attr_step(t_eclass* c, const char* attrname, float value)
{
    t_symbol* s_attrname = gensym(attrname);
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->step = value;
            return;
        }
    }
}

void eclass_attr_save(t_eclass* c, const char* attrname, long flags, bool value)
{
    t_symbol* s_attrname = gensym(attrname);
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->save = value ? 1 : 0;
            return;
        }
    }
}

void eclass_attr_paint(t_eclass* c, const char* attrname, long flags)
{
    t_symbol* s_attrname = gensym(attrname);
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->paint = 1;
            return;
        }
    }
}

void eclass_attr_invisible(t_eclass* c, const char* attrname, long flags)
{
    t_symbol* s_attrname = gensym(attrname);
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->invisible = 1;
            return;
        }
    }
}

void eclass_attr_accessor(t_eclass* c, const char* attrname, t_err_method getter, t_err_method setter)
{
    t_symbol* s_attrname = gensym(attrname);
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->getter = getter;
            c->c_attr[i]->setter = setter;
            return;
        }
    }
}

void eclass_attr_getter(t_object* x, t_symbol* s, int* argc, t_atom** argv)
{
    t_ebox* z = (t_ebox*)x;
    t_eclass* c = (t_eclass*)z->b_obj.o_obj.te_g.g_pd;

    if (argc == NULL) {
        pd_error(x, "[%s] null argc pointer given", class_getname(x->te_pd));
        return;
    }

    if (*argv) {
        pd_error(x, "[%s] invalid argv pointer given", class_getname(x->te_pd));
        return;
    }

    *argc = 0;

    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name != s)
            continue;

        char* point = (char*)x + c->c_attr[i]->offset;

        if (c->c_attr[i]->sizemax == 0) {
            *argc = (int)c->c_attr[i]->size;
        } else {
            point = (char*)x + c->c_attr[i]->size;
            *argc = (int)point[0];
            if (*argc > c->c_attr[i]->sizemax)
                *argc = (int)c->c_attr[i]->sizemax;
        }

        t_symbol* type = c->c_attr[i]->type;

        argv[0] = (t_atom*)getbytes((size_t)*argc * sizeof(t_atom));

        if (c->c_attr[i]->getter) {
            c->c_attr[i]->getter(x, c->c_attr[i], argc, argv);
        } else if (type == s_int) {
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
                atom_setfloat(argv[0] + j, (float)(((double*)point)[j]));
            }
        } else if (type == &s_symbol) {
            t_symbol** syms = (t_symbol**)point;
            for (int j = 0; j < *argc; j++) {
                if (syms[j]) {
                    atom_setsym(argv[0] + j, gensym(syms[j]->s_name));
                }
            }
        } else if (type == s_atom) {
            for (int j = 0; j < *argc; j++) {
                argv[0][j] = ((t_atom*)point)[j];
            }
        } else {
            printf("Unknown property get method: %s\n", type->s_name);
        }

        break;
    }
}

static bool request_property(t_object* x, t_symbol* s, std::vector<t_atom>& res)
{
    const size_t len = strlen(s->s_name);
    if (len < 3 || len > MAXPDSTRING) {
        pd_error(x, "[%s] invalid property name", class_getname(x->te_pd));
        return false;
    }

    int argc_ = 0;
    t_atom* argv_ = NULL;
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
    int argc_ = 0;
    t_atom* argv_ = NULL;
    t_ebox* z = (t_ebox*)x;
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

static void eclass_attr_ceammc_setter(t_object* x, t_symbol* s, int argc, t_atom* argv)
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

void eclass_attr_setter(t_object* x, t_symbol* s, int argc, t_atom* argv)
{
    long size;
    char* point;
    long* point_size;
    t_ebox* z = (t_ebox*)x;
    t_eclass* c = (t_eclass*)z->b_obj.o_obj.te_g.g_pd;

    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s) {
            t_symbol* type = c->c_attr[i]->type;
            if (c->c_attr[i]->sizemax == 0) {
                size = c->c_attr[i]->size;
            } else {
                if (argc > c->c_attr[i]->sizemax) {
                    argc = (int)c->c_attr[i]->sizemax;
                }
                size = argc;
                point = (char*)x + c->c_attr[i]->size;
                point_size = (long*)point;
                point_size[0] = (long)size;
            }

            point = (char*)(x) + c->c_attr[i]->offset;

            if (c->c_attr[i]->clipped == 1 || c->c_attr[i]->clipped == 3) {
                for (int j = 0; j < argc; j++) {
                    if (atom_gettype(argv + j) == A_FLOAT) {
                        atom_setfloat(argv + j, (float)pd_clip_min(atom_getfloat(argv + j), c->c_attr[i]->minimum));
                    }
                }
            }
            if (c->c_attr[i]->clipped == 2 || c->c_attr[i]->clipped == 3) {
                for (int j = 0; j < argc; j++) {
                    if (atom_gettype(argv + j) == A_FLOAT) {
                        atom_setfloat(argv + j, (float)pd_clip_max(atom_getfloat(argv + j), c->c_attr[i]->maximum));
                    }
                }
            }

            if (c->c_attr[i]->setter) {
                c->c_attr[i]->setter(x, c->c_attr[i], argc, argv);
            } else if (type == s_int) {
                int* pointor = (int*)point;
                for (int j = 0; j < size && j < argc; j++) {
                    if (atom_gettype(argv + j) == A_FLOAT) {
                        pointor[j] = (int)atom_getlong(argv + j);
                    }
                }
            } else if (type == s_long) {
                long* pointor = (long*)point;
                for (int j = 0; j < size && j < argc; j++) {
                    if (atom_gettype(argv + j) == A_FLOAT) {
                        pointor[j] = (long)atom_getlong(argv + j);
                    }
                }
            } else if (type == &s_float) {
                float* pointor = (float*)point;
                for (int j = 0; j < size && j < argc; j++) {
                    if (atom_gettype(argv + j) == A_FLOAT) {
                        pointor[j] = (float)atom_getfloat(argv + j);
                    }
                }
            } else if (type == s_double) {
                double* pointor = (double*)point;
                for (int j = 0; j < size && j < argc; j++) {
                    if (atom_gettype(argv + j) == A_FLOAT) {
                        pointor[j] = (double)atom_getfloat(argv + j);
                    }
                }
            } else if (type == &s_symbol) {
                t_symbol** pointor = (t_symbol**)point;
                for (int j = 0; j < size && j < argc; j++) {
                    if (atom_gettype(argv + j) == A_SYMBOL) {
                        pointor[j] = gensym(atom_getsymbol(argv + j)->s_name);
                    }
                }
            } else if (type == s_atom) {
                t_atom* pointor = (t_atom*)point;
                for (int j = 0; j < size && j < argc; j++) {
                    pointor[j] = argv[j];
                }
            }

            ebox_notify(z, s);
            if (c->c_widget.w_notify != NULL) {
                c->c_widget.w_notify(x, s, s_attr_modified);
            }

            if (c->c_attr[i]->paint) {
                if (c->c_widget.w_oksize != NULL) {
                    c->c_widget.w_oksize(x, &z->b_rect);
                }
                if (c->c_widget.w_getdrawparameters != NULL) {
                    c->c_widget.w_getdrawparameters(x, &z->b_boxparameters);
                }

                ebox_redraw(z);
            }
            if (c->c_attr[i]->save && eobj_isbox(x) && ebox_isdrawable((t_ebox*)x)) {
                canvas_dirty(eobj_getcanvas(x), 1);
            }
        }
    }
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
    c->c_widget.w_mouseenter = nullptr;
    c->c_widget.w_mouseleave = nullptr;
    c->c_widget.w_mousemove = nullptr;
    c->c_widget.w_mousedown = nullptr;
    c->c_widget.w_mousedrag = nullptr;
    c->c_widget.w_mouseup = nullptr;
    c->c_widget.w_mousewheel = nullptr;
    c->c_widget.w_dblclick = nullptr;
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

#ifdef __APPLE__
#define DIALOG_BACKGROUND " -background #ECECEC "
#else
#define DIALOG_BACKGROUND " -background #ECECEC "
#endif

static const char* dialog_frame_id(int i)
{
    static char buf[100];
    snprintf(buf, 100, "$id.t.frame%i", i);
    return buf;
}

static const char* dialog_label_id(int i)
{
    static char buf[100];
    snprintf(buf, 100, "$id.t.l%i", i);
    return buf;
}

static const char* dialog_widget_id(int i)
{
    static char buf[100];
    snprintf(buf, 100, "$id.t.w%i", i);
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

    t_symbol* s_color = gensym(SYM_COLOR);
    t_symbol* s_number = gensym(SYM_NUMBER);
    t_symbol* s_menu = gensym(SYM_MENU);
    t_symbol* s_checkbox = gensym(SYM_CHECKBUTTON);

    // DIALOG WINDOW APPLY //
    for (int i = 0; i < c->c_nattr; i++) {
        t_symbol* style = c->c_attr[i]->style;
        const char* prop_name = c->c_attr[i]->name->s_name;
        const char* class_name = c->c_class.c_name->s_name;

        if (style == s_color) {
            auto str = fmt::format(proc_color, class_name, prop_name, i + 1);
            sys_gui(str.c_str());
        } else if (style == s_number || style == s_menu || style == s_checkbox) {
            auto str = fmt::format(proc_num, class_name, prop_name, i + 1);
            sys_gui(str.c_str());
        } else {
            auto str = fmt::format(proc_entry, class_name, prop_name, i + 1);
            sys_gui(str.c_str());
        }
    }

    // DIALOG WINDOW CREATION //
    sys_vgui("proc pdtk_%s_dialog {id \n", c->c_class.c_name->s_name);
    for (int i = 0; i < c->c_nattr; i++) {
        const char* ATTR_NAME = c->c_attr[i]->name->s_name;
        if (!c->c_attr[i]->invisible) {
            sys_vgui("   %s \n", ATTR_NAME);
        }
    }
    sys_gui("} {\n");
    sys_gui("   set vid [string trimleft $id .]\n");

    /// fill category dict
#ifndef NDEBUG
    sys_gui("   # category dictionary");
#endif
    sys_gui("   set cat_dict [dict create]\n");
    for (int i = 0; i < c->c_nattr; i++) {
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
    sys_gui("   # global vars");
#endif
    for (int i = 0; i < c->c_nattr; i++) {
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
    sys_gui("   # window");
#endif
    // _("%s properties")
    auto str = fmt::format("   toplevel $id\n"
                           "   wm title $id [format [_ \"%s properties\" ] {{{0}}}] \n"
                           "   wm resizable $id 0 0\n"
                           "   raise [winfo toplevel $id]\n"
                           "   $id configure " DIALOG_BACKGROUND DIALOG_WINDOW_PADX DIALOG_WINDOW_PADY "\n"
                           "   frame $id.t\n"
                           "   grid $id.t\n",
        c->c_class.c_name->s_name);
    sys_gui(str.c_str());

    t_symbol* cat = &s_;
    int category_idx = 0;
    for (int i = 0; i < c->c_nattr; i++) {
        if (!c->c_attr[i]->invisible) {
            const char* FRAME_ID = dialog_frame_id(i + 1);
            const char* LABEL_ID = dialog_label_id(i + 1);
            const char* WIDGET_ID = dialog_widget_id(i + 1);
            const char* ATTR_NAME = c->c_attr[i]->name->s_name;
            const char* CLASS_NAME = c->c_class.c_name->s_name;

            /** PROPERTY CATEGORY **/
            if (c->c_attr[i]->category != cat) {
                auto str = fmt::format(
                    "   global var_cat{0}_state\n"
                    "   if {{ [info exists var_cat{0}_state] eq 0 }} {{ set var_cat{0}_state 1 }}\n"
                    "   ttk::label $id.t.cat_img{0} -image [ceammc_category_icon var_cat{0}_state]\n"
                    "   bind $id.t.cat_img{0} <Button> [list ceammc_category_toggle"
                    "       $id.t.cat_img{0} var_cat{0}_state [concat [dict get $cat_dict \"{0}\"]]]\n"
                    "   ttk::label $id.t.cat_lbl{0} -justify left -text [_ \"{0}\"] -font CICMCategoryFont\n"
                    "   grid config $id.t.cat_img{0} -column 0 -row {1} -sticky w\n"
                    "   grid config $id.t.cat_lbl{0} -column 1 -columnspan 2 -row {1} -sticky nwse\n",
                    c->c_attr[i]->category->s_name, i + category_idx + 1);
                // update current category
                cat = c->c_attr[i]->category;
                category_idx++;
                sys_gui(str.c_str());
            }

            /** ATTRIBUTES NAMES **/
            auto str = fmt::format("   # property: @{2}\n"
                                   "   ttk::label {0} -justify left -text [join [list [_ \"{1}\" ] {{:}}] {{}}]\n"
                                   "   tooltip::tooltip {0} \"@{2}\"\n",
                LABEL_ID, c->c_attr[i]->label->s_name, ATTR_NAME);
            sys_gui(str.c_str());

            /** SELECTOR WIDGETS **/
            if (c->c_attr[i]->style == gensym(SYM_CHECKBUTTON)) {
                auto str = fmt::format(
                    "   ttk::checkbutton {0} -variable [string trim $var_{2}] "
                    "   -command [concat pdtk_{1}_dialog_apply_{2} $id]\n",
                    WIDGET_ID, CLASS_NAME, ATTR_NAME);
                sys_gui(str.c_str());
            } else if (c->c_attr[i]->style == gensym(SYM_COLOR)) {
                auto str = fmt::format(
                    "   set color [eval eobj_rgba_to_hex ${2}]\n"
                    "   entry {0} -font {{Helvetica 11}} -width 10 -readonlybackground $color -state readonly\n"
                    "   bind  {0} <Button> [concat pdtk_{1}_picker_apply_{2} $id ${2}]\n",
                    WIDGET_ID, CLASS_NAME, ATTR_NAME);
                sys_gui(str.c_str());
            } else if (c->c_attr[i]->style == gensym(SYM_NUMBER)) {
                auto str = fmt::format(
                    "   ttk::spinbox {0} -width 18 -textvariable [string trim $var_{2}] -increment {3} \n"
                    "   {0} configure -command [concat pdtk_{1}_dialog_apply_{2} $id]\n"
                    "   {0} configure -from -9999999999999 -to 9999999999999\n"
                    "   {0} delete 0 end \n"
                    "   {0} insert 0 ${2} \n"
                    "   bind {0} <KeyPress-Return> [concat pdtk_{1}_dialog_apply_{2} $id]\n",
                    WIDGET_ID, CLASS_NAME, ATTR_NAME, (float)c->c_attr[i]->step);

                sys_gui(str.c_str());
            } else if (c->c_attr[i]->style == gensym(SYM_MENU)) {
                std::string tmpl(
                    "   ttk::combobox {0} -width 16 -state readonly -textvariable [string trim $var_{2}]\n"
                    "   {0} configure -values {{ ");

                // append values
                for (int j = 0; j < c->c_attr[i]->itemssize; j++) {
                    tmpl += c->c_attr[i]->itemslist[c->c_attr[i]->itemssize - 1 - j]->s_name;
                    tmpl.push_back(' ');
                }

                tmpl += "}}\n"
                        "   bind {0} <<ComboboxSelected>> [concat pdtk_{1}_dialog_apply_{2} $id]\n"
                        "   {0} set [string trim ${2}] \n";

                sys_gui(fmt::format(tmpl, WIDGET_ID, CLASS_NAME, ATTR_NAME).c_str());
            } else if (c->c_attr[i]->style == gensym(SYM_PATH)) {
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
            } else {
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
            }

            str = fmt::format(
                "   grid [ttk::frame {1}] -column 0 -row {0} -sticky nwse\n"
                "   grid {2} -column 1 -row {0} -sticky nwse\n"
                "   grid {3} -column 2 -row {0} -sticky nwse\n",
                i + category_idx + 1, FRAME_ID, LABEL_ID, WIDGET_ID);
            sys_gui(str.c_str());
        }
    }

    {
        // show/hide categories
        t_symbol* cat = &s_;
        for (int i = 0; i < c->c_nattr; i++) {
            if (c->c_attr[i]->invisible)
                continue;

            if (cat == c->c_attr[i]->category)
                continue;

            cat = c->c_attr[i]->category;
            str = fmt::format(
                "   if {{ $var_cat{0}_state eq 0 }} {{\n"
                "      set lst [dict get $cat_dict \"{0}\"]\n"
                "      ceammc_category_apply $id.t.cat_img{0} var_cat{0}_state $lst\n"
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

void eclass_attr_visible(t_eclass* c, const char* attrname, long flags)
{
    t_symbol* s_attrname = gensym(attrname);
    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == s_attrname) {
            c->c_attr[i]->invisible = 0;
            return;
        }
    }
}
