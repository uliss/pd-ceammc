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
#include <vector>

typedef struct _preset {
    t_ebox j_box;
    t_binbuf* f_binbuf[CREAM_MAXITEMS];
    int f_binbuf_selected;
    int f_binbuf_hover;
    float f_point_size;
    t_rgba color_background;
    t_rgba color_border;
    t_rgba f_color_button_stored;
    t_rgba f_color_button_empty;
    t_rgba f_color_button_selected;
    t_rgba f_color_text;
} t_preset;

static t_eclass* preset_class;

void preset_interpolate(t_preset* x, float f);

static void preset_store(t_preset* x, float f)
{
    int index = (int)f;
    if (index >= 1 && index < CREAM_MAXITEMS) {
        index -= 1;
        char id[MAXPDSTRING];
        t_atom av[2];

        t_binbuf* b = x->f_binbuf[index];
        if (binbuf_getnatom(b)) {
            binbuf_clear(b);
        }

        for (t_gobj* y = eobj_getcanvas(x)->gl_list; y; y = y->g_next) {
            t_ebox* z = (t_ebox*)y;
            t_gotfn mpreset = zgetfn(&y->g_pd, cream_sym_preset);
            if (mpreset && z->b_objpreset_id) {
                if (z->b_objpreset_id != s_null && z->b_objpreset_id != cream_sym_nothing) {
                    sprintf(id, "@%s", z->b_objpreset_id->s_name);
                    atom_setsym(av, gensym(id));
                    atom_setsym(av + 1, eobj_getclassname(z));
                    binbuf_add(b, 2, av);
                    mpreset(z, b);
                }
            }
            mpreset = NULL;
        }
    }
}

static void preset_float(t_preset* x, float f)
{
    t_canvas* cnv = eobj_getcanvas(x);
    if (cnv && !cnv->gl_loading) {
        x->f_binbuf_selected = pd_clip_minmax(f, 1, CREAM_MAXITEMS) - 1;
        t_binbuf* b = x->f_binbuf[x->f_binbuf_selected];
        if (b && binbuf_getnatom(b) && binbuf_getvec(b)) {
            char id[MAXPDSTRING];
            for (t_gobj* y = eobj_getcanvas(x)->gl_list; y; y = y->g_next) {
                t_ebox* z = (t_ebox*)y;
                t_gotfn mpreset = zgetfn(&y->g_pd, cream_sym_preset);
                if (mpreset && z->b_objpreset_id && z->b_objpreset_id != s_null && z->b_objpreset_id != cream_sym_nothing) {
                    int ac = 0;
                    t_atom* av = NULL;
                    sprintf(id, "@%s", z->b_objpreset_id->s_name);
                    binbuf_get_attribute(b, gensym(id), &ac, &av);
                    if (ac && av && atom_gettype(av) == A_SYMBOL && atom_gettype(av + 1) == A_SYMBOL) {
                        if (eobj_getclassname(z) == atom_getsymbol(av)) {
                            pd_typedmess((t_pd*)z, atom_getsymbol(av + 1), ac - 2, av + 2);
                        }
                    }
                    if (ac && av) {
                        free(av);
                    }
                }
                mpreset = NULL;
            }

            ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
            ebox_redraw((t_ebox*)x);
        }
    }
}

void preset_interpolate(t_preset* x, float f)
{
    t_gobj* y;
    t_ebox* z;
    t_gotfn mpreset = NULL;
    t_gotfn minterp = NULL;

    int acdo, acup, ac, max;
    t_atom *avdo, *avup, *av;
    char id[MAXPDSTRING];
    int i, j, indexdo, indexup, realdo, realup;
    float ratio;

    max = -1;
    for (i = CREAM_MAXITEMS - 1; i >= 0; i--) {
        if (binbuf_getnatom(x->f_binbuf[i])) {
            max = i + 1;
            break;
        }
    }
    if (max < 1)
        return;

    indexdo = pd_clip_minmax(floorf(f) - 1, 0, max - 1);
    indexup = pd_clip_minmax(ceilf(f) - 1, 0, max - 1);
    if (indexdo == indexup || f <= 1 || f >= max) {
        preset_float(x, f);
        return;
    }

    x->f_binbuf_selected = indexdo;

    // Look for all the objects in a canvas //
    for (y = eobj_getcanvas(x)->gl_list; y; y = y->g_next) {
        z = (t_ebox*)y;
        mpreset = zgetfn(&y->g_pd, cream_sym_preset);
        // We find a preset method so we can send preset //
        if (mpreset && z->b_objpreset_id && z->b_objpreset_id != s_null && z->b_objpreset_id != cream_sym_nothing) {
            sprintf(id, "@%s", z->b_objpreset_id->s_name);
            realdo = -1;
            realup = -1;
            acdo = 0;
            acup = 0;

            // Look for all the preset from the smallest index to zero //
            for (j = indexdo; j >= 0 && realdo == -1; j--) {
                // We find a recorded preset //
                if (binbuf_getnatom(x->f_binbuf[j])) {
                    // We get the preset //
                    binbuf_get_attribute(x->f_binbuf[j], gensym(id), &acdo, &avdo);
                    if (acdo >= 2 && avdo && atom_gettype(avdo) == A_SYMBOL && atom_gettype(avdo + 1) == A_SYMBOL) {
                        // If the object is in the preset we record the preset else we skip this preset //
                        if (eobj_getclassname(z) == atom_getsymbol(avdo)) {
                            realdo = j;
                            break;
                        } else {
                            avdo = NULL;
                            acdo = 0;
                        }
                    }
                }
            }

            // Look for all the preset from the biggest index to the top //
            for (j = indexup; j <= max && realup == -1; j++) {
                // We find a recorded preset //
                if (binbuf_getnatom(x->f_binbuf[j])) {
                    // We get the preset //
                    binbuf_get_attribute(x->f_binbuf[j], gensym(id), &acup, &avup);
                    if (acup >= 2 && avup && atom_gettype(avup) == A_SYMBOL && atom_gettype(avup + 1) == A_SYMBOL) {
                        // If the object is in the preset we record the preset else we skip this preset //
                        if (eobj_getclassname(z) == atom_getsymbol(avup)) {
                            realup = j;
                            break;
                        } else {
                            avup = NULL;
                            acup = 0;
                        }
                    }
                }
            }

            // If we have the 2 presets with the same selector for this object then we make an interpolation //
            if (acdo && acup && atom_getsymbol(avdo + 1) == atom_getsymbol(avup + 1)) {
                minterp = zgetfn(&y->g_pd, cream_sym_interpolate);
                if (minterp) {
                    t_atom theta;
                    ratio = (float)(f - (realdo + 1)) / (float)(realup - realdo);
                    atom_setfloat(&theta, ratio);
                    minterp((t_pd*)z, (short)acdo - 2, avdo + 2, (short)acup - 2, avup + 2, theta);
                } else {
                    ratio = (float)(f - (realdo + 1)) / (float)(realup - realdo);
                    ac = acdo;
                    av = (t_atom*)calloc((size_t)ac, sizeof(t_atom));
                    atom_setsym(av + 1, atom_getsymbol(avdo + 1));
                    for (j = 2; j < ac; j++) {
                        if (j < acup) {
                            if (atom_gettype(avdo + j) == A_FLOAT && atom_gettype(avup + j) == A_FLOAT) {
                                atom_setfloat(av + j, atom_getfloat(avdo + j) * (1. - ratio) + atom_getfloat(avup + j) * ratio);
                            } else {
                                av[j] = avdo[j];
                            }
                        } else {
                            av[j] = avdo[j];
                        }
                    }

                    pd_typedmess((t_pd*)z, atom_getsymbol(av + 1), ac - 2, av + 2);
                    free(av);
                }
            }
            // If we have only the smallest preset for this object //
            else if (acdo) {
                pd_typedmess((t_pd*)z, atom_getsymbol(avdo + 1), acdo - 2, avdo + 2);
            }
            // If we have only the highest preset for this object //
            else if (acup) {
                pd_typedmess((t_pd*)z, atom_getsymbol(avup + 1), acup - 2, avup + 2);
            }
        }
        mpreset = NULL;
    }

    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void preset_clear(t_preset* x, float f)
{
    int index = (int)f;
    if (index >= 1 && index < CREAM_MAXITEMS) {
        index -= 1;
        if (x->f_binbuf_selected == index) {
            x->f_binbuf_selected = 0;
        }
        binbuf_clear(x->f_binbuf[index]);

        ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
        ebox_redraw((t_ebox*)x);
    }
}

static void preset_clearall(t_preset* x)
{
    for (int i = 0; i < CREAM_MAXITEMS; i++) {
        binbuf_clear(x->f_binbuf[i]);
    }
    x->f_binbuf_selected = 0;
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void preset_getdrawparams(t_preset* x, t_object* patcherview, t_edrawparams* params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

static void preset_oksize(t_preset* x, t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

static t_pd_err preset_notify(t_preset* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
    if (msg == cream_sym_attr_modified) {
        if (s == cream_sym_bgcolor
            || s == cream_sym_bdcolor
            || s == cream_sym_btcolor
            || s == cream_sym_fontsize
            || s == cream_sym_fontname
            || s == cream_sym_fontweight
            || s == cream_sym_fontslant
            || s == gensym("textcolor")
            || s == gensym("stcolor")
            || s == gensym("emcolor")
            || s == gensym("secolor")) {
            ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
        }
        ebox_redraw((t_ebox*)x);
    }
    return 0;
}

static void draw_background(t_preset* x, t_object* view, t_rect* rect)
{
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_background_layer, rect->width, rect->height);
    t_etext* jtl = etext_layout_create();

    if (g && jtl) {
        char number[256];
        t_rgba color;

        int xc = x->f_point_size * 1.25f;
        int yc = x->f_point_size * 1.25f;
        for (int i = 1; i < CREAM_MAXITEMS; i++) {
            if (yc + x->f_point_size / 2.f >= rect->height)
                break;

            if (x->f_binbuf_selected == i - 1 && binbuf_getnatom(x->f_binbuf[i - 1])) {
                color = rgba_addContrast(x->f_color_button_selected, 0.1f);
            } else if (!binbuf_getnatom(x->f_binbuf[i - 1]))
                color = rgba_addContrast(x->f_color_button_empty, 0.1f);
            else if (binbuf_getnatom(x->f_binbuf[i - 1]))
                color = rgba_addContrast(x->f_color_button_stored, -0.1f);

            // on mouse over
            if (x->f_binbuf_hover == i) {
                color = rgba_addContrast(color, 0.08f);
            }

            egraphics_set_color_rgba(g, &color);
            egraphics_circle(g, xc, yc, x->f_point_size);
            egraphics_fill(g);

            sprintf(number, "%i", i);
            etext_layout_set(jtl, number, &x->j_box.b_font, xc, yc, rect->width, 0, ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP);
            etext_layout_settextcolor(jtl, &x->f_color_text);
            etext_layout_draw(jtl, g);

            xc += x->f_point_size * 2.5f;
            if (xc + x->f_point_size / 2.f > rect->width) {
                xc = x->f_point_size * 1.25f;
                yc += x->f_point_size * 2.5f;
            }
        }

        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_background_layer, 0., 0.);
}

static void preset_paint(t_preset* x, t_object* view)
{
    t_rect rect;
#ifdef __APPLE__
    x->j_box.b_font.c_size -= 3;
#elif _WINDOWS
    x->j_box.b_font.c_size -= 2;
#endif
    ebox_get_rect_for_view((t_ebox*)x, &rect);
    x->f_point_size = ebox_getzoomfontsize((t_ebox*)x);
    draw_background(x, view, &rect);

#ifdef __APPLE__
    x->j_box.b_font.c_size += 3;
#elif _WINDOWS
    x->j_box.b_font.c_size += 2;
#endif
}

static void preset_mousemove(t_preset* x, t_object* patcherview, t_pt pt, long modifiers)
{
    int index;
    int n_row_button = (x->j_box.b_rect.width - x->f_point_size * 1.24f) / (x->f_point_size * 2.5f) + 1;

    index = (int)((pt.y) / (x->f_point_size * 2.5f)) * n_row_button;
    index += pd_clip_max((pt.x) / (x->f_point_size * 2.5f) + 1, n_row_button);
    x->f_binbuf_hover = index;
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void preset_mousedown(t_preset* x, t_object* patcherview, t_pt pt, long modifiers)
{
    int index;
    int n_row_button = (x->j_box.b_rect.width - x->f_point_size * 1.24f) / (x->f_point_size * 2.5f) + 1;
    index = (int)((pt.y) / (x->f_point_size * 2.5f)) * n_row_button;
    index += pd_clip_max((pt.x) / (x->f_point_size * 2.5f) + 1, n_row_button);
    x->f_binbuf_hover = index;

    if (modifiers == EMOD_ALT)
        preset_clear(x, index);
    if (modifiers == EMOD_SHIFT)
        preset_store(x, index);
    preset_float(x, index);
}

static void preset_mouseleave(t_preset* x, t_object* patcherview, t_pt pt, long modifiers)
{
    x->f_binbuf_hover = 0;
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void preset_save(t_preset* x, t_binbuf* d)
{
    binbuf_addv(d, "ss", cream_sym_atpreset, cream_sym_left_bracket);
    for (int i = 0; i < CREAM_MAXITEMS; i++) {
        if (binbuf_getnatom(x->f_binbuf[i])) {
            binbuf_addv(d, (char*)"sf", cream_sym_atindex, (float)i);
            binbuf_addbinbuf(d, x->f_binbuf[i]);
        }
    }
    binbuf_addv(d, (char*)"s", cream_sym_right_bracket);
}

static void preset_init(t_preset* x, t_binbuf* d)
{
    int check;
    long index;
    int ac = binbuf_getnatom(d);
    t_atom* av = binbuf_getvec(d);
    for (int i = 0; i < ac; i++) {
        if (atom_gettype(av + i) == A_SYMBOL && atom_getsymbol(av + i) == cream_sym_atpreset) {
            for (; i < ac; i++) {
                if (atom_gettype(av + i) == A_SYMBOL && atom_getsymbol(av + i) == cream_sym_atindex) {
                    i++;
                    if (i + 1 < ac && atom_gettype(av + i) == A_FLOAT) {
                        index = atom_getlong(av + i);
                        binbuf_clear(x->f_binbuf[index]);
                        i++;
                        check = 1;
                        for (; i < ac && check; i++) {
                            if (atom_gettype(av + i) == A_SYMBOL && atom_getsymbol(av + i) == cream_sym_atindex) {
                                i -= 2;
                                check = 0;
                            } else if (atom_gettype(av + i) == A_SYMBOL && atom_getsymbol(av + i) == cream_sym_right_bracket) {
                                return;
                            } else {
                                binbuf_add(x->f_binbuf[index], 1, av + i);
                            }
                        }
                    }
                }
            }
        }
    }
}

static void preset_read(t_preset* x, t_symbol* s, int argc, t_atom* argv)
{
    t_binbuf* d = binbuf_new();
    if (d && argv && argc && atom_gettype(argv) == A_SYMBOL) {
        if (binbuf_read(d, atom_getsymbol(argv)->s_name, (char*)"", 0)) {
            pd_error(x, "preset : %s read failed", atom_getsymbol(argv)->s_name);
        } else {
            preset_init(x, d);
            ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
            ebox_redraw((t_ebox*)x);
            post("preset : read %s.", atom_getsymbol(argv)->s_name);
        }
    }
    if (d) {
        binbuf_free(d);
    }
}

static void preset_write(t_preset* x, t_symbol* s, int argc, t_atom* argv)
{
    t_binbuf* d = binbuf_new();
    if (d && argv && argc && atom_gettype(argv) == A_SYMBOL) {
        preset_save(x, d);
        if (binbuf_write(d, atom_getsymbol(argv)->s_name, (char*)"", 0)) {
            pd_error(x, "preset : %s write failed", atom_getsymbol(argv)->s_name);
        } else {
            post("preset : write %s.", atom_getsymbol(argv)->s_name);
        }
    }
    if (d) {
        binbuf_free(d);
    }
}

static void* preset_new(t_symbol* s, int argc, t_atom* argv)
{
    t_preset* x = (t_preset*)eobj_new(preset_class);
    t_binbuf* d = binbuf_via_atoms(argc, argv);
    if (x && d) {
        for (int i = 0; i < CREAM_MAXITEMS; i++) {
            x->f_binbuf[i] = binbuf_new();
        }
        ebox_new((t_ebox*)x, 0 | EBOX_GROWINDI);

        x->f_binbuf_selected = 0;
        x->f_binbuf_hover = 0;

        preset_init(x, d);
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox*)x);

        binbuf_free(d);
    }

    return (x);
}

static void preset_free(t_preset* x)
{
    for (int i = 0; i < CREAM_MAXITEMS; i++) {
        binbuf_free(x->f_binbuf[i]);
    }

    ebox_free((t_ebox*)x);
}

extern "C" void setup_ui0x2epreset(void)
{
    t_eclass* c = eclass_new("ui.preset", (method)preset_new, (method)preset_free, (short)sizeof(t_preset), 0L, A_GIMME, 0);

    if (c) {
        // clang-format off
        eclass_guiinit(c, 0);

        eclass_addmethod(c, (method) preset_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) preset_notify,          "notify",           A_NULL, 0);
        eclass_addmethod(c, (method) preset_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) preset_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) preset_store,           "store",            A_FLOAT,0);
        eclass_addmethod(c, (method) preset_clear,           "clear",            A_FLOAT,0);
        eclass_addmethod(c, (method) preset_float,           "float",            A_FLOAT,0);
        eclass_addmethod(c, (method) preset_interpolate,     "inter",            A_FLOAT,0);
        eclass_addmethod(c, (method) preset_clearall,        "clearall",         A_NULL,0);

        eclass_addmethod(c, (method) preset_mousemove,       "mousemove",        A_NULL, 0);
        eclass_addmethod(c, (method) preset_mousedown,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) preset_mouseleave,      "mouseleave",       A_NULL, 0);

        eclass_addmethod(c, (method) preset_save,            "save",             A_NULL, 0);
        eclass_addmethod(c, (method) preset_read,            "read",             A_GIMME,0);
        eclass_addmethod(c, (method) preset_write,           "write",            A_GIMME,0);

        CLASS_ATTR_INVISIBLE            (c, "send", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        CLASS_ATTR_DEFAULT              (c, "size", 0, "102 42");
        CLASS_ATTR_DEFAULT              (c, "fontsize", 0, "11");

        CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_preset, color_background);
        CLASS_ATTR_LABEL                (c, "bgcolor", 0, _("Background Color"));
        CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.8 0.8 0.8 1.");
        CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");

        ATTR_DEFAULT_COLOR_BORDER       (c, t_preset);

        CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_preset, f_color_text);
        CLASS_ATTR_LABEL                (c, "textcolor", 0, _("Text Color"));
        CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
        CLASS_ATTR_STYLE                (c, "textcolor", 0, "color");

        CLASS_ATTR_RGBA                 (c, "emcolor", 0, t_preset, f_color_button_empty);
        CLASS_ATTR_LABEL                (c, "emcolor", 0, _("Empty Button Color"));
        CLASS_ATTR_ORDER                (c, "emcolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "emcolor", 0, "0.86 0.86 0.86 1.");
        CLASS_ATTR_STYLE                (c, "emcolor", 0, "color");

        CLASS_ATTR_RGBA                 (c, "stcolor", 0, t_preset, f_color_button_stored);
        CLASS_ATTR_LABEL                (c, "stcolor", 0, _("Stored Button Color"));
        CLASS_ATTR_ORDER                (c, "stcolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "stcolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "stcolor", 0, "color");

        CLASS_ATTR_RGBA                 (c, "secolor", 0, t_preset, f_color_button_selected);
        CLASS_ATTR_LABEL                (c, "secolor", 0, _("Active Color"));
        CLASS_ATTR_ORDER                (c, "secolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "secolor", 0, DEFAULT_ACTIVE_COLOR);
        CLASS_ATTR_STYLE                (c, "secolor", 0, "color");

        // clang-format on

        eclass_register(CLASS_BOX, c);
        preset_class = c;
    }
}
