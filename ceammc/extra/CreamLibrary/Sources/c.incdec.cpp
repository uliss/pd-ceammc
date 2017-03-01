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

typedef struct _incdec {
    t_ebox j_box;

    t_outlet* f_out;
    float f_increment;
    float f_value;
    t_rgba color_background;
    t_rgba color_border;
    t_rgba f_color_arrow;
    int f_mouse_down;
    char f_setted;

} t_incdec;

static t_eclass* incdec_class;

static void* incdec_new(t_symbol* s, int argc, t_atom* argv);
static void incdec_free(t_incdec* x);

static void incdec_set(t_incdec* x, float f);
static void incdec_float(t_incdec* x, float f);
static void incdec_output(t_incdec* x);
static void incdec_inc(t_incdec* x);
static void incdec_dec(t_incdec* x);

static t_pd_err incdec_notify(t_incdec* x, t_symbol* s, t_symbol* msg, void* sender, void* data);
static void incdec_getdrawparams(t_incdec* x, t_object* patcherview, t_edrawparams* params);
static void incdec_oksize(t_incdec* x, t_rect* newrect);
static void incdec_paint(t_incdec* x, t_object* view);
static void draw_background(t_incdec* x, t_object* view, t_rect* rect);
static void incdec_mousedown(t_incdec* x, t_object* patcherview, t_pt pt, long modifiers);
static void incdec_mouseup(t_incdec* x, t_object* patcherview, t_pt pt, long modifiers);

static void incdec_get_value(t_incdec* x, t_object* /*attr*/, long* ac, t_atom** av)
{
    *ac = 1;
    *av = reinterpret_cast<t_atom*>(calloc(1, sizeof(t_atom)));
    atom_setfloat(*av, x->f_value);
}

static t_pd_err incdec_set_value(t_incdec* x, t_object* /*attr*/, int ac, t_atom* av)
{
    if (ac > 0 && av) {
        incdec_set(x, atom_getfloat(av));
        return 0;
    }

    return 1;
}

extern "C" void setup_ui0x2eincdec(void)
{
    t_eclass* c = eclass_new("ui.incdec", (method)incdec_new, (method)incdec_free, sizeof(t_incdec), 0L, A_GIMME, 0);

    eclass_guiinit(c, 0);
    eclass_addmethod(c, (method)incdec_paint, "paint", A_NULL, 0);
    eclass_addmethod(c, (method)incdec_notify, "notify", A_NULL, 0);
    eclass_addmethod(c, (method)incdec_getdrawparams, "getdrawparams", A_NULL, 0);
    eclass_addmethod(c, (method)incdec_oksize, "oksize", A_NULL, 0);
    eclass_addmethod(c, (method)incdec_set, "set", A_FLOAT, 0);
    eclass_addmethod(c, (method)incdec_float, "float", A_FLOAT, 0);
    eclass_addmethod(c, (method)incdec_output, "bang", A_NULL, 0);
    eclass_addmethod(c, (method)incdec_inc, "inc", A_NULL, 0);
    eclass_addmethod(c, (method)incdec_dec, "dec", A_NULL, 0);
    eclass_addmethod(c, (method)incdec_mousedown, "mousedown", A_NULL, 0);
    eclass_addmethod(c, (method)incdec_mouseup, "mouseup", A_NULL, 0);

    // clang-format off
    CLASS_ATTR_INVISIBLE              (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE              (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE              (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE              (c, "fontsize", 1);
    CLASS_ATTR_DEFAULT                (c, "size", 0, "13 20");

    CLASS_ATTR_FLOAT                  (c, "step", 0, t_incdec, f_increment);
    CLASS_ATTR_LABEL                  (c, "step", 0, _("Step increment"));
    CLASS_ATTR_FILTER_MIN             (c, "step", 0.);
    CLASS_ATTR_ORDER                  (c, "step", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT     (c, "step", 0, "1.");
    CLASS_ATTR_STYLE                  (c, "step", 0, "number");
    CLASS_ATTR_STEP                   (c, "step", 0.1);

    ATTR_DEFAULT_COLOR_BORDER         (c, t_incdec);
    ATTR_DEFAULT_COLOR_BACKGROUND     (c, t_incdec);

    CLASS_ATTR_RGBA                   (c, "arrow_color", 0, t_incdec, f_color_arrow);
    CLASS_ATTR_LABEL                  (c, "arrow_color", 0, _("Arrow Color"));
    CLASS_ATTR_ORDER                  (c, "arrow_color", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT     (c, "arrow_color", 0, DEFAULT_BORDER_COLOR);
    CLASS_ATTR_STYLE                  (c, "arrow_color", 0, "color");
    // clang-format on

    CLASS_ATTR_VIRTUAL              (c,  "value", incdec_get_value, incdec_set_value);


    eclass_register(CLASS_BOX, c);
    incdec_class = c;
}

void* incdec_new(t_symbol* s, int argc, t_atom* argv)
{
    t_incdec* x = NULL;
    t_binbuf* d;
    long flags;
    if (!(d = binbuf_via_atoms(argc, argv)))
        return NULL;

    x = (t_incdec*)eobj_new(incdec_class);
    flags = 0 | EBOX_GROWINDI;
    ebox_new((t_ebox*)x, flags);

    x->f_value = 0.;
    x->f_mouse_down = 0;
    x->f_out = outlet_new((t_object*)x, &s_float);
    ebox_attrprocess_viabinbuf(x, d);
    x->f_setted = 0;
    ebox_ready((t_ebox*)x);

    binbuf_free(d);
    return (x);
}

void incdec_getdrawparams(t_incdec* x, t_object* patcherview, t_edrawparams* params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

void incdec_oksize(t_incdec* x, t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void incdec_set(t_incdec* x, float f)
{
    x->f_setted = 1;
    x->f_value = f;
}

void incdec_float(t_incdec* x, float f)
{
    if (x->f_value != f || x->f_setted == 1) {
        x->f_value = f;
        incdec_output(x);
    }
}

void incdec_output(t_incdec* x)
{
    x->f_setted = 0;
    outlet_float((t_outlet*)x->f_out, (float)x->f_value);
}

void incdec_inc(t_incdec* x)
{
    x->f_value += x->f_increment;
    incdec_output(x);
}

void incdec_dec(t_incdec* x)
{
    x->f_value -= x->f_increment;
    incdec_output(x);
}

void incdec_free(t_incdec* x)
{
    ebox_free((t_ebox*)x);
}

t_pd_err incdec_notify(t_incdec* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
    if (msg == cream_sym_attr_modified) {
        if (s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == gensym("arcolor")) {
            ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
        }
        ebox_redraw((t_ebox*)x);
    }
    return 0;
}

void incdec_paint(t_incdec* x, t_object* view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)x, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_incdec* x, t_object* view, t_rect* rect)
{
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_background_layer, rect->width, rect->height);

    if (g) {
        const float height = roundf(rect->height / 2.f);

        // Background //
        egraphics_set_color_rgba(g, &x->f_color_arrow);
        if (x->f_mouse_down == 1)
            egraphics_rectangle(g, 0, 0, rect->width, height);
        else if (x->f_mouse_down == -1)
            egraphics_rectangle(g, 0, height, rect->width, rect->height);
        egraphics_fill(g);

        egraphics_set_color_rgba(g, &x->f_color_arrow);

        // Arrow Up //
        if (x->f_mouse_down == 1)
            egraphics_set_color_rgba(g, &x->color_background);
        else
            egraphics_set_color_rgba(g, &x->f_color_arrow);

        const int arrow_p0_x = static_cast<int>(roundf(pd_clip_min(rect->width * 0.2f, 2)));
        const int arrow_p1_x = rect->width - arrow_p0_x;
        const int arrow_p2_x = static_cast<int>(roundf(rect->width * 0.5f));

        const int arrow_p0_y = static_cast<int>(roundf(pd_clip_min(height * 0.2f, 2)));
        const int arrow_p1_y = arrow_p0_y;
        const int arrow_p2_y = height - arrow_p0_y;

        egraphics_move_to(g, arrow_p0_x, height - arrow_p0_y);
        egraphics_line_to(g, arrow_p1_x, height - arrow_p1_y);
        egraphics_line_to(g, arrow_p2_x, arrow_p0_y);
        egraphics_line_to(g, arrow_p0_x, height - arrow_p0_y);
        egraphics_fill(g);

        // Arrow Down //
        if (x->f_mouse_down == -1)
            egraphics_set_color_rgba(g, &x->color_background);
        else
            egraphics_set_color_rgba(g, &x->f_color_arrow);

        egraphics_move_to(g, arrow_p0_x, height + arrow_p0_y);
        egraphics_line_to(g, arrow_p1_x, height + arrow_p1_y);
        egraphics_line_to(g, arrow_p2_x, height + arrow_p2_y);
        egraphics_line_to(g, arrow_p0_x, height + arrow_p0_y);
        egraphics_fill(g);

        // Middle Line //
        egraphics_set_color_rgba(g, &x->color_border);
        egraphics_set_line_width(g, 1);
        egraphics_line_fast(g, 0, height, rect->width, height);

        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_background_layer, 0, 0);
}

void incdec_mousedown(t_incdec* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (pt.y < x->j_box.b_rect.height / 2.) {
        incdec_inc(x);
        x->f_mouse_down = 1;
    } else {
        incdec_dec(x);
        x->f_mouse_down = -1;
    }
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

void incdec_mouseup(t_incdec* x, t_object* patcherview, t_pt pt, long modifiers)
{
    x->f_mouse_down = 0;
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}
