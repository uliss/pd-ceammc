/*
 * Cream Library
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CreamLibrary
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include <cmath>

#include "../c.library.hpp"
#include <boost/static_assert.hpp>

BOOST_STATIC_ASSERT((sizeof(t_edspbox) - sizeof(t_edsp)) == sizeof(t_ebox));

typedef struct _dsp_tilde {
    t_ebox j_box;
    char f_state;
    char f_init;
    t_rgba color_background;
    t_rgba color_border;
    t_rgba color_active;

} t_dsp_tilde;

static t_eclass* dsp_tildeclass;

static void* dsp_tilde_new(t_symbol* s, int argc, t_atom* argv)
{
    long flags;
    t_dsp_tilde* x = (t_dsp_tilde*)eobj_new(dsp_tildeclass);
    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if (x && d) {
        flags = 0 | EBOX_GROWLINK;

        ebox_new((t_ebox*)x, flags);
        eobj_proxynew(x);
        x->f_init = 0;
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox*)x);
        pd_bind((t_pd*)x, gensym("pd"));

        binbuf_free(d);

        return x;
    }

    return NULL;
}

static void dsp_tilde_getdrawparams(t_dsp_tilde* x, t_object* patcherview, t_edrawparams* params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

static void dsp_tilde_oksize(t_dsp_tilde* x, t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 30.);
    newrect->height = pd_clip_min(newrect->height, 30.);
}

static void dsp_tilde_free(t_dsp_tilde* x)
{
    pd_unbind((t_pd*)x, gensym("pd"));
    ebox_free((t_ebox*)x);
}

static void draw_background(t_dsp_tilde* x, t_object* view, t_rect* rect)
{
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_background_layer, rect->width, rect->height);

    if (g) {
        if (x->f_state) {
            egraphics_set_color_rgba(g, &x->color_active);
        } else {
            egraphics_set_color_rgba(g, &x->color_border);
        }

        const float center = roundf(rect->width * 0.5f - 0.5f);
        egraphics_circle(g, center, center, roundf(rect->width * 0.15f - 0.5f));
        egraphics_fill(g);

        egraphics_set_line_width(g, 2.);
        egraphics_arc(g, center, center, roundf(rect->width * 0.25f - 0.5f), 0, EPD_PI);
        egraphics_stroke(g);

        egraphics_arc(g, center, center, roundf(rect->width * 0.35f - 0.5f), 0, EPD_PI);
        egraphics_stroke(g);

        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_background_layer, 0.f, 0.f);
}

static void dsp_tilde_paint(t_dsp_tilde* x, t_object* view)
{
    t_rect rect;
    if (!x->f_init) {
        x->f_state = canvas_dspstate;
        x->f_init = 1;
    }

    ebox_get_rect_for_view((t_ebox*)x, &rect);
    draw_background(x, view, &rect);
}

static void dsp_tilde_anything(t_dsp_tilde* x, t_symbol* s, int argc, t_atom* argv)
{
    if (s == gensym("dsp") && argc && atom_gettype(argv) == A_FLOAT) {
        x->f_state = atom_getfloat(argv);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
        ebox_redraw((t_ebox*)x);
    }
}

static void dsp_tilde_open(t_dsp_tilde* x)
{
    sys_gui((char*)"pdsend \"pd audio-properties\"\n");
}

static void dsp_tilde_start(t_dsp_tilde* x)
{
    t_atom av;
    atom_setfloat(&av, 1);
    pd_typedmess((t_pd*)gensym("pd")->s_thing, gensym("dsp"), 1, &av);
    x->f_state = 1;
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void dsp_tilde_stop(t_dsp_tilde* x)
{
    t_atom av;
    atom_setfloat(&av, 0);
    pd_typedmess((t_pd*)gensym("pd")->s_thing, gensym("dsp"), 1, &av);
    x->f_state = 0;
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void dsp_tilde_dblclick(t_dsp_tilde* x, t_object*, t_pt, long modifiers)
{
    if (modifiers == EMOD_SHIFT) {
        dsp_tilde_open(x);
    }
}

static void dsp_tilde_mousedown(t_dsp_tilde* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (canvas_dspstate) {
        dsp_tilde_stop(x);
    } else {
        dsp_tilde_start(x);
    }
}

extern "C" void setup_ui0x2edsp_tilde(void)
{
    t_eclass* c;

    c = eclass_new("ui.dsp~", (method)dsp_tilde_new, (method)dsp_tilde_free, (short)sizeof(t_dsp_tilde), CLASS_NOINLET, A_GIMME, 0);

    // clang-format off
    eclass_guiinit(c, 0);

    eclass_addmethod(c, (method) dsp_tilde_paint,           "paint",            A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_getdrawparams,   "getdrawparams",    A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_oksize,          "oksize",           A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_mousedown,       "mousedown",        A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_anything,        "anything",         A_GIMME, 0);
    eclass_addmethod(c, (method) dsp_tilde_open,            "settings",         A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_start,           "start",            A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_stop,            "stop",             A_NULL, 0);

    eclass_addmethod(c, (method) dsp_tilde_dblclick,        "dblclick",         A_NULL, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
    CLASS_ATTR_DEFAULT              (c, "size", 0, "30 30");

    ATTR_DEFAULT_COLOR_BORDER       (c, t_dsp_tilde);
    ATTR_DEFAULT_COLOR_BACKGROUND   (c, t_dsp_tilde);
    ATTR_DEFAULT_COLOR_ACTIVE       (c, t_dsp_tilde);

    // clang-format on
    eclass_register(CLASS_BOX, c);
    dsp_tildeclass = c;
}
