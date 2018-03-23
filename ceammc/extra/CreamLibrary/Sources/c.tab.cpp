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

typedef struct _tab {
    t_ebox j_box;

    t_outlet* f_out_index;
    t_outlet* f_out_item;
    t_outlet* f_out_hover;

    t_symbol* f_items[CREAM_MAXITEMS];
    long f_nitems;
    long f_item_selected;
    long f_item_hover;

    long f_toggle;
    long f_orientation;
    t_rgba color_background;
    t_rgba color_border;
    t_rgba color_text;
    t_rgba color_active;
    t_rgba f_color_hover;
} t_tab;

static t_eclass* tab_class;

static t_symbol* atoms_getsymbol(int ac, t_atom* av)
{
    char buffer[MAXPDSTRING];
    memset(buffer, '\0', MAXPDSTRING * sizeof(char));
    for (int i = 0; i < ac; i++) {
        if (atom_gettype(av + i) == A_SYMBOL) {
            const t_symbol* s = atom_getsymbol(av + i);
            const int length = (int)strlen(s->s_name);
            int buflen = (int)strlen(buffer);
            if (buflen) {
                buffer[buflen++] = ' ';
            }
            for (int j = 0, k = buflen; j < length && k < MAXPDSTRING - 1; j++) {
                if (s->s_name[j] != '"' && s->s_name[j] != '\'') {
                    buffer[k++] = s->s_name[j];
                }
            }
            buffer[MAXPDSTRING - 1] = '\0';
        } else if (atom_gettype(av + i) == A_FLOAT) {
            char temp[MAXPDSTRING];
            sprintf(temp, "%f", atom_getfloat(av + i));
            int lenght = (int)strlen(temp);
            while (temp[lenght - 1] == '0') {
                temp[lenght - 1] = '\0';
                lenght--;
            }
            if (temp[lenght - 1] == '.') {
                temp[lenght - 1] = '\0';
                lenght--;
            }
            if (buffer[0] != '\0') {
                strncat(buffer, " ", (size_t)1);
            }
            strncat(buffer, temp, (size_t)lenght);
            memset(temp, '\0', MAXPDSTRING * sizeof(char));
        }
    }
    return gensym(buffer);
}

static int tab_getindex(t_tab* x, t_symbol* s)
{
    if (s) {
        for (int i = 0; i < x->f_nitems; i++) {
            if (s == x->f_items[i]) {
                return i;
            }
        }
    }
    return -1;
}

static void tab_append(t_tab* x, t_symbol* s, int argc, t_atom* argv)
{
    t_symbol* item = atoms_getsymbol(argc, argv);
    if (item && tab_getindex(x, item) == -1) {
        x->f_items[x->f_nitems] = item;
        x->f_nitems++;

        ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
        ebox_redraw((t_ebox*)x);
    }
}

static void tab_insert(t_tab* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc > 1 && argv && atom_gettype(argv) == A_FLOAT) {
        const int index = atom_getfloat(argv);
        if (index >= 0 && index < x->f_nitems && index < CREAM_MAXITEMS) {
            t_symbol* item = atoms_getsymbol(argc - 1, argv + 1);
            if (item && tab_getindex(x, item) == -1) {
                for (int i = (int)x->f_nitems; i > index; i--) {
                    x->f_items[i] = x->f_items[i - 1];
                }
                x->f_items[index] = item;
                x->f_nitems++;

                ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
                ebox_redraw((t_ebox*)x);
            }
        } else if (index >= 0) {
            t_symbol* item = atoms_getsymbol(argc - 1, argv + 1);
            if (item && tab_getindex(x, item) == -1) {
                x->f_items[x->f_nitems] = item;
                x->f_nitems++;

                ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
                ebox_redraw((t_ebox*)x);
            }
        }
    }
}

static void tab_setitem(t_tab* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc > 1 && argv && atom_gettype(argv) == A_FLOAT) {
        const int index = atom_getfloat(argv);
        if (index >= 0 && index < x->f_nitems) {
            t_symbol* item = atoms_getsymbol(argc - 1, argv + 1);
            if (item && tab_getindex(x, item) == -1) {
                x->f_items[index] = item;
                ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
                ebox_redraw((t_ebox*)x);
            }
        }
    }
}

static void tab_delete(t_tab* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc && argv) {
        if (atom_gettype(argv) == A_FLOAT) {
            const int index = (int)atom_getfloat(argv);
            if (index >= 0 && index < x->f_nitems) {
                for (int i = index; i < x->f_nitems - 1; i++) {
                    x->f_items[i] = x->f_items[i + 1];
                }
                x->f_nitems--;

                ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
                ebox_redraw((t_ebox*)x);
            }
        } else {
            const int index = tab_getindex(x, atoms_getsymbol(argc, argv));
            if (index >= 0 && index < x->f_nitems) {
                for (int i = index; i < x->f_nitems - 1; i++) {
                    x->f_items[i] = x->f_items[i + 1];
                }
                x->f_nitems--;

                ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
                ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
                ebox_redraw((t_ebox*)x);
            }
        }
    }
}

static void tab_clear(t_tab* x, t_symbol* s, int argc, t_atom* argv)
{
    for (int i = 0; i < CREAM_MAXITEMS; i++) {
        x->f_items[i] = NULL;
    }
    x->f_nitems = 0;

    ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void tab_output(t_tab* x)
{
    if (x->f_nitems > 0) {
        t_pd* send = ebox_getsender((t_ebox*)x);
        outlet_float(x->f_out_index, x->f_item_selected);
        if (x->f_item_selected != -1) {
            outlet_symbol(x->f_out_item, x->f_items[x->f_item_selected]);
        }
        outlet_float(x->f_out_hover, x->f_item_hover);

        if (send) {
            pd_float(send, (float)x->f_item_selected);
        }
    }
}

static void tab_setfloat(t_tab* x, t_floatarg f)
{
    if (x->f_toggle && f >= 0 && f < x->f_nitems) {
        x->f_item_selected = f;
        ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
        ebox_redraw((t_ebox*)x);
    }
}

static void tab_setsymbol(t_tab* x, t_symbol* s)
{
    if (x->f_toggle) {
        x->f_item_selected = tab_getindex(x, s);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
        ebox_redraw((t_ebox*)x);
    }
}

static void tab_float(t_tab* x, t_floatarg f)
{
    if (x->f_toggle) {
        tab_setfloat(x, f);
        tab_output(x);
    } else if (f >= 0 && f < x->f_nitems) {
        x->f_item_selected = f;
        tab_output(x);
        x->f_item_selected = -1;
    }
}

static void tab_symbol(t_tab* x, t_symbol* s, int argc, t_atom* argv)
{
    t_atom* av = (t_atom*)malloc((size_t)(argc + 1) * sizeof(t_atom));
    if (av) {
        atom_setsym(av, s);
        memcpy(av + 1, argv, (size_t)(argc) * sizeof(t_atom));
        t_symbol* item = atoms_getsymbol(argc + 1, av);
        post(item->s_name);
        if (item && x->f_toggle && tab_getindex(x, item) != -1) {
            post(item->s_name);
            x->f_item_selected = tab_getindex(x, item);
            tab_output(x);
            ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
            ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
            ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
            ebox_redraw((t_ebox*)x);
        } else if (item) {
            x->f_item_selected = tab_getindex(x, item);
            tab_output(x);
            x->f_item_selected = -1;
        }
        free(av);
    }
}

static void tab_set(t_tab* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc && argv) {
        if (atom_gettype(argv) == A_FLOAT) {
            tab_setfloat(x, atom_getfloat(argv));
        } else if (atom_gettype(argv) == A_SYMBOL) {
            tab_setsymbol(x, atoms_getsymbol(argc, argv));
        }
    }
}

static void tab_getdrawparams(t_tab* x, t_object* patcherview, t_edrawparams* params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

static void tab_oksize(t_tab* x, t_rect* newrect)
{
    //    if (x->f_orientation) {
    //        newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.b_font.c_size) * 3);
    //        newrect->height = pd_clip_min(newrect->height, (sys_fontheight(x->j_box.b_font.c_size) + 4) * pd_clip_min(x->f_nitems, 1));
    //    } else {
    //        newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.b_font.c_size) * 3 * pd_clip_min(x->f_nitems, 1));
    //        newrect->height = pd_clip_min(newrect->height, sys_fontheight(x->j_box.b_font.c_size) + 4);
    //    }
}

static t_pd_err tab_notify(t_tab* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
    if (msg == cream_sym_attr_modified) {
        if (s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_textcolor || s == cream_sym_hocolor || s == cream_sym_secolor || s == cream_sym_orientation || s == cream_sym_fontsize || s == cream_sym_fontname || s == cream_sym_fontweight || s == cream_sym_fontslant) {
            ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
            ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
            ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
        } else if (s == cream_sym_fontsize || s == cream_sym_orientation || s == cream_sym_items) {
            ebox_notify((t_ebox*)x, s_size, cream_sym_attr_modified, NULL, NULL);
        }
    }
    return 0;
}

static void draw_background(t_tab* x, t_object* view, t_rect* rect)
{
    int i;
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_background_layer, rect->width, rect->height);
    if (g) {
        egraphics_set_color_rgba(g, &x->color_border);
        egraphics_set_line_width(g, 2);
        if (x->f_orientation) {
            const float ratio = rect->height / (float)x->f_nitems;
            for (i = 1; i < x->f_nitems; i++) {
                egraphics_line_fast(g, 0, ratio * i, rect->width, ratio * i);
            }
        } else {
            const float ratio = rect->width / (float)x->f_nitems;
            for (i = 1; i < x->f_nitems; i++) {
                egraphics_line_fast(g, ratio * i, 0, ratio * i, rect->height);
            }
        }
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_background_layer, 0., 0.);
}

static void draw_selection(t_tab* x, t_object* view, t_rect* rect)
{
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_selection_layer, rect->width, rect->height);
    if (g) {
        egraphics_set_line_width(g, 2);
        if (x->f_orientation) {
            const float ratio = rect->height / (float)x->f_nitems;
            if (x->f_item_hover != -1) {
                egraphics_set_color_rgba(g, &x->f_color_hover);
                egraphics_rectangle(g, 0, ratio * x->f_item_hover, rect->width, ratio);
                egraphics_fill(g);
            }
            if (x->f_item_selected != -1) {
                egraphics_set_color_rgba(g, &x->color_active);
                egraphics_rectangle(g, 0, ratio * x->f_item_selected, rect->width, ratio);
                egraphics_fill(g);
            }
        } else {
            const float ratio = rect->width / (float)x->f_nitems;
            if (x->f_item_hover != -1) {
                egraphics_set_color_rgba(g, &x->f_color_hover);
                egraphics_rectangle(g, ratio * x->f_item_hover, 0, ratio, rect->height);
                egraphics_fill(g);
            }
            if (x->f_item_selected != -1) {
                egraphics_set_color_rgba(g, &x->color_active);
                egraphics_rectangle(g, ratio * x->f_item_selected, 0, ratio, rect->height);
                egraphics_fill(g);
            }
        }
        ebox_end_layer((t_ebox*)x, cream_sym_selection_layer);
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_selection_layer, 0., 0.);
}

static void draw_text(t_tab* x, t_object* view, t_rect* rect)
{
    int i;
    t_elayer* g = ebox_start_layer((t_ebox*)x, cream_sym_text_layer, rect->width, rect->height);
    if (g) {
        t_etext* jtl = etext_layout_create();
        if (jtl) {
            if (x->f_orientation) {
                const float ratio = rect->height / (float)x->f_nitems;
                for (i = 0; i < x->f_nitems; i++) {
                    if (x->f_items[i] && x->f_items[i] != s_null) {
                        etext_layout_set(jtl, x->f_items[i]->s_name, &x->j_box.b_font,
                            rect->width * 0.5,
                            ratio * (i + 0.5),
                            rect->width,
                            0, ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP);
                        etext_layout_draw(jtl, g);
                    }
                }
            } else {
                const float ratio = rect->width / (float)x->f_nitems;
                for (i = 0; i < x->f_nitems; i++) {
                    if (x->f_items[i] && x->f_items[i] != s_null) {
                        etext_layout_settextcolor(jtl, &x->color_text);
                        etext_layout_set(jtl, x->f_items[i]->s_name, &x->j_box.b_font,
                            ratio * (i + 0.5),
                            rect->height * 0.5,
                            ratio - 2,
                            0, ETEXT_CENTER, ETEXT_JCENTER, ETEXT_WRAP);
                        etext_layout_draw(jtl, g);
                    }
                }
            }
            etext_layout_destroy(jtl);
        }
        ebox_end_layer((t_ebox*)x, cream_sym_text_layer);
    }
    ebox_paint_layer((t_ebox*)x, cream_sym_text_layer, 0., 0.);
}

static void tab_paint(t_tab* x, t_object* view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)x, &rect);

    draw_selection(x, view, &rect);
    draw_background(x, view, &rect);
    draw_text(x, view, &rect);
}

static void tab_mousedown(t_tab* x, t_object* patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)x, &rect);
    const int index = x->f_orientation ? (pt.y / (rect.height / (float)x->f_nitems)) : (pt.x / (rect.width / (float)x->f_nitems));
    x->f_item_selected = pd_clip_minmax(index, 0, x->f_nitems - 1);

    tab_output(x);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void tab_mouseup(t_tab* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (!x->f_toggle) {
        x->f_item_selected = -1;
        ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
        ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
        ebox_redraw((t_ebox*)x);
    }
}

static void tab_mouseleave(t_tab* x, t_object* patcherview, t_pt pt, long modifiers)
{
    x->f_item_hover = -1;

    outlet_float(x->f_out_hover, x->f_item_hover);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void tab_mousemove(t_tab* x, t_object* patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)x, &rect);
    const int index = x->f_orientation ? (pt.y / (rect.height / (float)x->f_nitems)) : (pt.x / (rect.width / (float)x->f_nitems));
    x->f_item_hover = pd_clip_minmax(index, 0, x->f_nitems - 1);

    outlet_float(x->f_out_hover, x->f_item_hover);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_selection_layer);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_text_layer);
    ebox_invalidate_layer((t_ebox*)x, cream_sym_background_layer);
    ebox_redraw((t_ebox*)x);
}

static void tab_preset(t_tab* x, t_binbuf* b)
{
    binbuf_addv(b, (char*)"sf", &s_float, (float)x->f_item_selected);
}

static void* tab_new(t_symbol* s, int argc, t_atom* argv)
{
    t_tab* x = (t_tab*)eobj_new(tab_class);
    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if (x && d) {
        ebox_new((t_ebox*)x, 0 | EBOX_GROWINDI);

        x->f_out_index = outlet_new((t_object*)x, &s_float);
        x->f_out_item = outlet_new((t_object*)x, &s_list);
        x->f_out_hover = outlet_new((t_object*)x, &s_float);

        x->f_item_selected = -1;
        x->f_item_hover = -1;
        x->f_nitems = 0;
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox*)x);

        binbuf_free(d);
    }

    return (x);
}

extern "C" void setup_ui0x2etab(void)
{
    t_eclass* c = eclass_new("ui.tab", (method)tab_new, (method)ebox_free, (short)sizeof(t_tab), 0L, A_GIMME, 0);

    // clang-format off
    if(c) {

        eclass_guiinit(c, 0);
        eclass_addmethod(c, (method) tab_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) tab_notify,          "notify",           A_NULL, 0);
        eclass_addmethod(c, (method) tab_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) tab_oksize,          "oksize",           A_NULL, 0);
        
        eclass_addmethod(c, (method) tab_append,          "append",           A_GIMME,0);
        eclass_addmethod(c, (method) tab_insert,          "insert",           A_GIMME,0);
        eclass_addmethod(c, (method) tab_setitem,         "setitem",          A_GIMME,0);
        eclass_addmethod(c, (method) tab_delete,          "delete",           A_GIMME,0);
        eclass_addmethod(c, (method) tab_clear,           "clear",            A_GIMME,0);
        
        eclass_addmethod(c, (method) tab_float,           "float",            A_FLOAT,0);
        eclass_addmethod(c, (method) tab_symbol,          "anything",         A_GIMME,0);
        eclass_addmethod(c, (method) tab_set,             "set",              A_GIMME,0);
        eclass_addmethod(c, (method) tab_output,          "bang",             A_NULL, 0);
        
        eclass_addmethod(c, (method) tab_mousedown,        "mousedown",       A_NULL, 0);
        eclass_addmethod(c, (method) tab_mouseup,          "mouseup",         A_NULL, 0);
        eclass_addmethod(c, (method) tab_mousemove,        "mousemove",       A_NULL, 0);
        eclass_addmethod(c, (method) tab_mouseleave,       "mouseleave",      A_NULL, 0);
        eclass_addmethod(c, (method) tab_preset,           "preset",          A_NULL, 0);
        
        CLASS_ATTR_DEFAULT              (c, "size", 0, "100 16");
        
        CLASS_ATTR_LONG                 (c, "orientation", 0, t_tab, f_orientation);
        CLASS_ATTR_LABEL                (c, "orientation", 0, _("Vertical Orientation"));
        CLASS_ATTR_ORDER                (c, "orientation", 0, "1");
        CLASS_ATTR_FILTER_CLIP          (c, "orientation", 0, 1);
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "orientation", 0, "0");
        CLASS_ATTR_STYLE                (c, "orientation", 0, "onoff");
        
        CLASS_ATTR_LONG                 (c, "toggle", 0, t_tab, f_toggle);
        CLASS_ATTR_LABEL                (c, "toggle", 0, _("Toggle Mode"));
        CLASS_ATTR_ORDER                (c, "toggle", 0, "1");
        CLASS_ATTR_FILTER_CLIP          (c, "toggle", 0, 1);
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "toggle", 0, "0");
        CLASS_ATTR_STYLE                (c, "toggle", 0, "onoff");
        
        CLASS_ATTR_SYMBOL_VARSIZE       (c, "items", 0, t_tab, f_items, f_nitems, CREAM_MAXITEMS);
        CLASS_ATTR_LABEL                (c, "items", 0, _("Items"));
        CLASS_ATTR_ORDER                (c, "items", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "items", 0, "");
        
        ATTR_DEFAULT_COLOR_BORDER       (c, t_tab);
        ATTR_DEFAULT_COLOR_BACKGROUND   (c, t_tab);
        ATTR_DEFAULT_COLOR_ACTIVE       (c, t_tab);
        ATTR_DEFAULT_COLOR_TEXT         (c, t_tab);

        CLASS_ATTR_RGBA                 (c, "hocolor", 0, t_tab, f_color_hover);
        CLASS_ATTR_LABEL                (c, "hocolor", 0, _("Hover Color"));
        CLASS_ATTR_ORDER                (c, "hocolor", 0, "4");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "hocolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "hocolor", 0, "color");
        
        eclass_register(CLASS_BOX, c);
        tab_class = c;

        // clang-format on
    }
}
