/*
 * Cream Library
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CreamLibrary
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "../c.library.hpp"

#include <algorithm>
#include <stdlib.h>

typedef struct _number {
    t_ebox j_box;

    t_outlet* f_outlet;
    char f_mode;

    float f_value;
    float f_refvalue;
    float f_deriv;
    float f_inc;

    char f_textvalue[CREAM_MAXITEMS];
    t_atom f_min;
    t_atom f_max;

    t_rgba color_background;
    t_rgba color_border;
    t_rgba color_text;

public:
    void set(float f)
    {
        if (atom_gettype(&f_max) == A_FLOAT && atom_gettype(&f_min) == A_FLOAT) {
            f_value = pd_clip_minmax(f, atom_getfloat(&f_min), atom_getfloat(&f_max));
        } else if (atom_gettype(&f_max) == A_FLOAT) {
            f_value = pd_clip_max(f, atom_getfloat(&f_max));
        } else if (atom_gettype(&f_min) == A_FLOAT) {
            f_value = pd_clip_min(f, atom_getfloat(&f_min));
        } else {
            f_value = f;
        }
    }

    void setMin(const t_atom& a)
    {
        f_min = a;
        if (atom_gettype(&f_max) == A_FLOAT && atom_getfloat(&f_max) < atom_getfloat(&f_min))
            std::swap(f_max, f_min);

        set(f_value);
    }

    void setMax(const t_atom& a)
    {
        f_max = a;
        if (atom_gettype(&f_min) == A_FLOAT && atom_getfloat(&f_max) < atom_getfloat(&f_min))
            std::swap(f_max, f_min);

        set(f_value);
    }
} t_number;

static t_symbol* SYM_PLUS = gensym("+");
static t_symbol* SYM_MINUS = gensym("-");
static t_symbol* SYM_MUL = gensym("*");
static t_symbol* SYM_DIV = gensym("/");
static t_symbol* SYM_INC = gensym("++");
static t_symbol* SYM_DEC = gensym("--");

static t_eclass* number_class;

static void number_output(t_number* x)
{
    t_pd* send = ebox_getsender((t_ebox*)x);
    outlet_float(x->f_outlet, x->f_value);
    if (send) {
        pd_float(send, x->f_value);
    }
}

static void number_float(t_number* x, float f)
{
    x->set(f);
    number_output(x);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
    ebox_redraw((t_ebox*)x);
}

static void number_set(t_number* x, float f)
{
    x->set(f);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
    ebox_redraw((t_ebox*)x);
}

static t_pd_err number_notify(t_number* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
    if (msg == cream_sym_attr_modified) {
        if (s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_textcolor || s == cream_sym_fontsize || s == cream_sym_fontname || s == cream_sym_fontweight || s == cream_sym_fontslant) {
            ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
            ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
        }
        if (s == cream_sym_fontsize || s == cream_sym_fontname) {
            ebox_notify((t_ebox*)x, s_size, cream_sym_attr_modified, NULL, NULL);
        }
    }
    return 0;
}

static void number_getdrawparams(t_number* x, t_object* patcherview, t_edrawparams* params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

static void number_oksize(t_number* x, t_rect* newrect)
{
    const float size = ebox_getzoomfontsize((t_ebox*)x);
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(size) * 3 + 8);
    newrect->height = size + 4;
}

static void draw_background(t_number* x, t_object* view, t_rect* rect)
{
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_background_layer, rect->width, rect->height);
    if (g) {
        const float width = rect->height * 0.4f;
        egraphics_set_line_width(g, 1);
        egraphics_set_color_rgba(g, &x->color_border);
        egraphics_move_to(g, 0, 0);
        egraphics_line_to(g, width, rect->height * 0.5f);
        egraphics_line_to(g, 0, rect->height);
        egraphics_stroke(g);
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_background_layer, 0., 0.);
}

static void draw_value_drag(t_number* x, t_object* view, t_rect* rect)
{
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_value_layer, rect->width, rect->height);
    if (g) {
        t_etext* jtl = etext_layout_create();
        if (jtl) {
            const float width = rect->height * 0.45f + 2;
            char number[256];
            snprintf(number, 256, "%g", x->f_value);
            etext_layout_settextcolor(jtl, &x->color_text);
            etext_layout_set(jtl, number, &x->j_box.b_font, width, rect->height * 0.5f, rect->width - width, rect->height, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);

            etext_layout_draw(jtl, g);
            ebox_end_layer((t_ebox*)x, cream_sym_value_layer);
            etext_layout_destroy(jtl);
        }
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_value_layer, 0., 0.);
}

static void draw_value_text(t_number* x, t_object* view, t_rect* rect)
{
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_value_layer, rect->width, rect->height);
    if (g) {
        t_etext* jtl = etext_layout_create();
        if (jtl) {
            const float width = sys_fontwidth(ebox_getzoomfontsize((t_ebox*)x)) + 8;
            char number[256];

            sprintf(number, "%s|", x->f_textvalue);
            etext_layout_settextcolor(jtl, &x->color_text);

            etext_layout_set(jtl, number, &x->j_box.b_font, width, rect->height / 2., rect->width - 3, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);

            etext_layout_draw(jtl, g);
            ebox_end_layer((t_ebox*)x, cream_sym_value_layer);
            etext_layout_destroy(jtl);
        }
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_value_layer, 0., 0.);
}

static void number_paint(t_number* x, t_object* view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)x, &rect);

    draw_background(x, view, &rect);
    if (!x->f_mode)
        draw_value_drag(x, view, &rect);
    else
        draw_value_text(x, view, &rect);
}

void number_mousedown(t_number* x, t_object* patcherview, t_pt pt, long modifiers)
{
    float text_width = sys_fontwidth(ebox_getzoomfontsize((t_ebox*)x));
    x->f_mode = 0;
    if (pt.x >= text_width + 6) {
        int i = 1;
        int n_integer = 1;
        float pos = pt.x - text_width + 8 / text_width;
        x->f_deriv = pt.y;
        x->f_refvalue = x->f_value;
        while (fabs(x->f_refvalue) >= powf(10, n_integer))
            n_integer++;

        while (text_width + 6 + i * text_width < pos)
            i++;

        if (x->f_refvalue < 0) // due to "-" offset
        {
            if (i < n_integer)
                x->f_inc = -powf(10, (n_integer - i));
            else
                x->f_inc = -1. / powf(10, (i - n_integer - 1));
        } else {
            if (i < n_integer + 2)
                x->f_inc = -powf(10, (n_integer - i));
            else
                x->f_inc = -1. / powf(10, (i - n_integer - 1));
        }

        x->f_inc = pd_clip_minmax(x->f_inc, -100., -0.000001);
    }
}

static void number_modify_single(t_number* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 || !argv) {
        pd_error(x, "[%s] %s: float argument required", eobj_getclassname(x)->s_name, s->s_name);
        return;
    }

    if (s == SYM_PLUS) {
        number_set(x, x->f_value + atom_getfloat(argv));
    } else if (s == SYM_MINUS) {
        number_set(x, x->f_value - atom_getfloat(argv));
    } else if (s == SYM_MUL) {
        number_set(x, x->f_value * atom_getfloat(argv));
    } else if (s == SYM_DIV) {
        t_float v = atom_getfloat(argv);
        if (v == 0.f) {
            pd_error(x, "[%s] division by zero attempt.", eobj_getclassname(x)->s_name);
            return;
        }

        number_set(x, x->f_value / v);
    }
}

static void number_modify(t_number* x, t_symbol* s)
{
    if (s == SYM_INC) {
        number_set(x, x->f_value + 1);
    } else if (s == SYM_DEC) {
        number_set(x, x->f_value - 1);
    }
}

void number_mousedrag(t_number* x, t_object* patcherview, t_pt pt, long modifiers)
{
    x->f_mode = 0;
    ebox_set_cursor((t_ebox*)x, 2);
    float value;
    if (modifiers == EMOD_SHIFT)
        value = x->f_refvalue + (pt.y - x->f_deriv) * x->f_inc * 0.01;
    else
        value = x->f_refvalue + (pt.y - x->f_deriv) * x->f_inc * 0.5;

    if (PD_BADFLOAT(value) || PD_BIGORSMALL(value))
        return;

    x->set(value);
    number_output(x);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
    ebox_redraw((t_ebox*)x);
}

void number_dblclick(t_number* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (x->f_mode == 0) {
        x->f_mode = 1;
        //sprintf(x->f_textvalue, "");
        memset(x->f_textvalue, '\0', 256 * sizeof(char));
        ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
        ebox_redraw((t_ebox*)x);
    }
}

void number_key(t_number* x, t_object* patcherview, char textcharacter, long modifiers)
{
    if (!x->f_mode || strlen(x->f_textvalue) >= 256) {
        if (textcharacter == 'R') {
            x->f_value++;
            number_output(x);
        } else if (textcharacter == 'T') {
            x->f_value--;
            number_output(x);
        }

    } else {
        if (textcharacter == '-' && strlen(x->f_textvalue) == 0) {
            strncat(x->f_textvalue, &textcharacter, 1);
        } else if (textcharacter == '.') {
            if (atof(x->f_textvalue) - atoi(x->f_textvalue) == 0 && x->f_textvalue[strlen(x->f_textvalue) - 1] != '.') {
                strncat(x->f_textvalue, &textcharacter, 1);
            }
        } else if (isdigit(textcharacter)) {
            strncat(x->f_textvalue, &textcharacter, 1);
        }
    }

    ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
    ebox_redraw((t_ebox*)x);
}

void number_keyfilter(t_number* x, t_object* patcherview, char textcharacter, long modifiers)
{
    if (!x->f_mode)
        return;

    if (textcharacter == EKEY_DEL) {
        int lenght = (int)strlen(x->f_textvalue);
        if (lenght > 1) {
            memset(x->f_textvalue + lenght - 1, '\0', (size_t)(CREAM_MAXITEMS - lenght + 1) * sizeof(char));
        } else {
            memset(x->f_textvalue, '\0', CREAM_MAXITEMS * sizeof(char));
        }

        ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
        ebox_redraw((t_ebox*)x);
    } else if (textcharacter == EKEY_TAB || textcharacter == EKEY_ENTER) {
        x->f_mode = 0;
        x->set(atof(x->f_textvalue));
        number_output(x);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
        ebox_redraw((t_ebox*)x);
    } else if (textcharacter == EKEY_ESC) {
        x->f_mode = 0;
        memset(x->f_textvalue, '\0', CREAM_MAXITEMS * sizeof(char));
        ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
        ebox_redraw((t_ebox*)x);
    }
}

void number_mouseleave(t_number* x)
{
    x->f_mode = 0;
    memset(x->f_textvalue, '\0', CREAM_MAXITEMS * sizeof(char));
    ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
    ebox_redraw((t_ebox*)x);
}

static void number_preset(t_number* x, t_binbuf* b)
{
    binbuf_addv(b, (char*)"sf", &s_float, x->f_value);
}

t_pd_err number_min_set(t_number* x, t_object* attr, int ac, t_atom* av)
{
    if (ac && av && atom_gettype(av) == A_FLOAT) {
        x->setMin(av[0]);
    } else {
        atom_setsym(&x->f_min, s_null);
    }

    ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
    ebox_redraw((t_ebox*)x);
    return 0;
}

t_pd_err number_max_set(t_number* x, t_object* attr, int ac, t_atom* av)
{
    if (ac && av && atom_gettype(av) == A_FLOAT) {
        x->setMax(av[0]);
    } else {
        atom_setsym(&x->f_max, s_null);
    }

    ebox_invalidate_layer((t_ebox*)x, cream_sym_value_layer);
    ebox_redraw((t_ebox*)x);
    return 0;
}

static void* number_new(t_symbol* s, int argc, t_atom* argv)
{
    t_number* x = (t_number*)eobj_new(number_class);
    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if (x && d) {
        ebox_new((t_ebox*)x, 0 | EBOX_GROWINDI);
        x->f_outlet = outlet_new((t_object*)x, &s_float);
        x->f_mode = 0.f;
        sprintf(x->f_textvalue, "0.");
        x->set(0);
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox*)x);

        binbuf_free(d);

        return x;
    }

    return NULL;
}

extern "C" void setup_ui0x2enumber(void)
{
    t_eclass* c = eclass_new("ui.number", (method)number_new, (method)ebox_free, (short)sizeof(t_number), 0L, A_GIMME, 0);

    if (c) {
        // clang-format off
        eclass_guiinit(c, 0);
        eclass_addmethod(c, (method) number_paint,            "paint",           A_NULL,  0);
        eclass_addmethod(c, (method) number_notify,           "notify",          A_NULL,  0);
        eclass_addmethod(c, (method) number_getdrawparams,    "getdrawparams",   A_NULL,  0);
        eclass_addmethod(c, (method) number_oksize,           "oksize",          A_NULL,  0);
        eclass_addmethod(c, (method) number_float,            "float",           A_FLOAT, 0);
        eclass_addmethod(c, (method) number_set,              "set",             A_FLOAT, 0);
        eclass_addmethod(c, (method) number_output,           "bang",            A_NULL,  0);
        
        eclass_addmethod(c, (method) number_mousedown,        "mousedown",       A_NULL,  0);
        eclass_addmethod(c, (method) number_mousedrag,        "mousedrag",       A_NULL,  0);
        eclass_addmethod(c, (method) number_dblclick,         "dblclick",        A_NULL,  0);
        eclass_addmethod(c, (method) number_key,              "key",             A_NULL,  0);
        eclass_addmethod(c, (method) number_keyfilter,        "keyfilter",       A_NULL,  0);
        eclass_addmethod(c, (method) number_mouseleave,       "mouseleave",      A_NULL,  0);
        
        eclass_addmethod(c, (method) number_preset,           "preset",          A_NULL,  0);
        eclass_addmethod(c, (method) number_modify_single,    "+",               A_GIMME, 0);
        eclass_addmethod(c, (method) number_modify_single,    "-",               A_GIMME, 0);
        eclass_addmethod(c, (method) number_modify_single,    "*",               A_GIMME, 0);
        eclass_addmethod(c, (method) number_modify_single,    "/",               A_GIMME, 0);
        eclass_addmethod(c, (method) number_modify,           "++",              A_GIMME, 0);
        eclass_addmethod(c, (method) number_modify,           "--",              A_GIMME, 0);
        
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);

        CLASS_ATTR_DEFAULT			    (c, "size", 0, "53 16");
        
        CLASS_ATTR_ATOM                 (c, "min", 0, t_number, f_min);
        CLASS_ATTR_ORDER                (c, "min", 0, "3");
        CLASS_ATTR_LABEL                (c, "min", 0, _("Minimum Value"));
        CLASS_ATTR_DEFAULT              (c, "min", 0, "(null)");
        CLASS_ATTR_ACCESSORS            (c, "min", NULL, number_min_set);
        CLASS_ATTR_SAVE                 (c, "min", 1);

        CLASS_ATTR_ATOM                 (c, "max", 0, t_number, f_max);
        CLASS_ATTR_ORDER                (c, "max", 0, "3");
        CLASS_ATTR_LABEL                (c, "max", 0, _("Maximum Value"));
        CLASS_ATTR_DEFAULT              (c, "max", 0, "(null)");
        CLASS_ATTR_ACCESSORS            (c, "max", NULL, number_max_set);
        CLASS_ATTR_SAVE                 (c, "max", 1);
        
        ATTR_DEFAULT_COLOR_BORDER       (c, t_number);
        ATTR_DEFAULT_COLOR_BACKGROUND   (c, t_number);
        ATTR_DEFAULT_COLOR_TEXT         (c, t_number);

        // clang-format on

        eclass_register(CLASS_BOX, c);
        number_class = c;
    }
}
