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

typedef struct _keyboard
{
	t_ebox      j_box;
    t_outlet*   f_out_note;
    t_outlet*   f_out_velo;
	t_rgba		f_color_wkeys;
    t_rgba		f_color_bkeys;
    t_rgba		f_color_skeys;
	t_rgba		f_color_border;
    t_symbol*   f_mode;
    long        f_low_key;
    long        f_high_key;
    long        f_nwhite_keys;
    long        f_selected_key;
} t_keyboard;

t_eclass *keyboard_class;

static t_pd_err keyboard_is_blackkey(long key)
{
    const long index = key % 12;
    return index == 1 || index == 3 || index == 6 || index == 8 || index == 10;
}

static long keyboard_limit_key(long key, long val)
{
    return key > val ? key : val;
}

static long keyboard_count_whitekey(long down, long up)
{
    long counter = 0;
    for(long i = down; i <= up; i++)
    {
        if(!keyboard_is_blackkey(i))
        {
            counter++;
        }
    }
    return counter;
}

static void keyboard_output(t_keyboard *x)
{
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
    //outlet_float(x->f_out, (float)x->f_active);
    //if(ebox_getsender((t_ebox *) x))
    //    pd_float(ebox_getsender((t_ebox *) x), (float)x->f_active);
}

static void keyboard_getdrawparams(t_keyboard *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_wkeys;
}

static void keyboard_oksize(t_keyboard *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.f);
    newrect->height = pd_clip_min(newrect->height, 15.f);
    if(newrect->width / (float)x->f_nwhite_keys < 5.f)
    {
        newrect->width = (float)x->f_nwhite_keys * 5.f;
    }
}

static void keyboard_set(t_keyboard *x, float f)
{

    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

static void keyboard_float(t_keyboard *x, float f)
{

    keyboard_output(x);
}

static void keyboard_bang(t_keyboard *x)
{

    keyboard_output(x);
}


static t_pd_err keyboard_notify(t_keyboard *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if(msg == cream_sym_attr_modified)
	{
		if(s == cream_sym_wkeycolor || s == cream_sym_bkeycolor)
		{
			ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
		}
        else if(s == cream_sym_skeycolor)
        {
            ebox_invalidate_layer((t_ebox *)x, cream_sym_selection_layer);
        }
	}
	return 0;
}

static void draw_background(t_keyboard *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect->width, rect->height);
	if(g)
	{
        const float width = rect->width / (float)x->f_nwhite_keys;
        for(long i = 0; i < x->f_nwhite_keys; i++)
        {
            egraphics_rectangle(g, (float)i * width, 0., width, rect->height);
            egraphics_set_color_rgba(g, &x->f_color_border);
            egraphics_stroke_preserve(g);
            egraphics_set_color_rgba(g, &x->f_color_wkeys);
            egraphics_fill(g);
        }
        const float bwidth = pd_clip_max(width * 0.9f, width - 2.f);
        for(long i = x->f_low_key, j = 0; i <= x->f_high_key; i++)
        {
            if(keyboard_is_blackkey(i))
            {
                egraphics_rectangle(g, (float)j * width - bwidth * 0.5, 0., bwidth, rect->height * 0.5);
                egraphics_set_color_rgba(g, &x->f_color_border);
                egraphics_stroke_preserve(g);
                egraphics_set_color_rgba(g, &x->f_color_bkeys);
                egraphics_fill(g);
            }
            else
            {
                j++;
            }
        }
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
	}
	ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0., 0.);
}

static void keyboard_paint(t_keyboard *x, t_object *view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

static void keyboard_mousedown(t_keyboard *x, t_object *patcherview, t_pt pt, long modifiers)
{
    keyboard_bang(x);
}

static void keyboard_mousedrag(t_keyboard *x, t_object *patcherview, t_pt pt, long modifiers)
{
    keyboard_bang(x);
}

static void keyboard_mouseup(t_keyboard *x, t_object *patcherview, t_pt pt, long modifiers)
{
    keyboard_bang(x);
}

static void keyboard_preset(t_keyboard *x, t_binbuf *b)
{
    t_atom av[2];
    atom_setsym(av, &s_list);
    //atom_setfloat(av+1, x->f_active);
    binbuf_add(b, 2, av);
}

static t_pd_err keyboard_lowkey_set(t_keyboard *x, t_object *attr, int ac, t_atom *av)
{
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        x->f_low_key  = keyboard_limit_key((long)atom_getfloat(av), 0);
        if(keyboard_is_blackkey(x->f_low_key))
        {
            x->f_low_key--;
        }
        x->f_high_key = keyboard_limit_key(x->f_high_key, x->f_low_key + 1);
        if(keyboard_is_blackkey(x->f_high_key))
        {
            x->f_high_key++;
        }
        x->f_nwhite_keys = keyboard_count_whitekey(x->f_low_key, x->f_high_key);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_selection_layer);
    }
    return 0;
}

static t_pd_err keyboard_mode_set(t_keyboard *x, t_object *attr, int ac, t_atom *av)
{
    if(ac && av)
    {
        if(atom_gettype(av) == A_SYMBOL)
        {
            if(atom_getsymbol(av) == gensym("touchscreen") || atom_getsymbol(av) == gensym("polyphonic"))
            {
                x->f_mode = atom_getsymbol(av);
            }
            else
            {
                 x->f_mode = gensym("monophonic");
            }
        }
        else if(atom_gettype(av) == A_FLOAT)
        {
            if(atom_getfloat(av) == 2)
            {
                x->f_mode = gensym("touchscreen");
            }
            else if(atom_getfloat(av) == 1)
            {
                x->f_mode = gensym("polyphonic");
            }
            else
            {
                x->f_mode = gensym("monophonic");
            }
        }
        ebox_invalidate_layer((t_ebox *)x, cream_sym_selection_layer);
    }
    return 0;
}

static t_pd_err keyboard_highkey_set(t_keyboard *x, t_object *attr, int ac, t_atom *av)
{
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        x->f_high_key = keyboard_limit_key((long)atom_getfloat(av), x->f_low_key + 1);
        if(keyboard_is_blackkey(x->f_high_key))
        {
            x->f_high_key++;
        }
        x->f_nwhite_keys = keyboard_count_whitekey(x->f_low_key, x->f_high_key);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_selection_layer);
    }
    return 0;
}

static void *keyboard_new(t_symbol *s, int argc, t_atom *argv)
{
    t_keyboard *x = (t_keyboard *)eobj_new(keyboard_class);
    t_binbuf* d = binbuf_via_atoms(argc, argv);
    
    if(x && d)
    {
        ebox_new((t_ebox *)x, 0 | EBOX_GROWINDI);
        x->f_out_note = outlet_new((t_object *)x, &s_float);
        x->f_out_velo = outlet_new((t_object *)x, &s_float);
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
    }
    
    return (x);
}

extern "C" void setup_c0x2ekeyboard(void)
{
    t_eclass *c;
    
    c = eclass_new("c.keyboard", (method)keyboard_new, (method)ebox_free, (short)sizeof(t_keyboard), 0L, A_GIMME, 0);
    
    eclass_guiinit(c, 0);
    eclass_addmethod(c, (method) keyboard_paint,           "paint",            A_NULL, 0);
    eclass_addmethod(c, (method) keyboard_notify,          "notify",           A_NULL, 0);
    eclass_addmethod(c, (method) keyboard_getdrawparams,   "getdrawparams",    A_NULL, 0);
    eclass_addmethod(c, (method) keyboard_oksize,          "oksize",           A_NULL, 0);
    eclass_addmethod(c, (method) keyboard_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) keyboard_set,             "set",              A_FLOAT,0);
    eclass_addmethod(c, (method) keyboard_bang,            "bang",             A_NULL, 0);
    eclass_addmethod(c, (method) keyboard_mousedown,       "mousedown",        A_NULL, 0);
    eclass_addmethod(c, (method) keyboard_mousedrag,       "mousedrag",        A_NULL, 0);
    eclass_addmethod(c, (method) keyboard_mouseup,         "mouseup",          A_NULL, 0);
    eclass_addmethod(c, (method) keyboard_preset,          "preset",           A_NULL, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
    CLASS_ATTR_DEFAULT              (c, "size", 0, "300. 50.");
    
    CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_keyboard, f_color_border);
    CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
    CLASS_ATTR_ORDER                (c, "bdcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "wkeycolor", 0, t_keyboard, f_color_wkeys);
    CLASS_ATTR_LABEL                (c, "wkeycolor", 0, "White Key Color");
    CLASS_ATTR_ORDER                (c, "wkeycolor", 0, "2");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "wkeycolor", 0, "1. 1. 1. 1.");
    CLASS_ATTR_STYLE                (c, "wkeycolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "bkeycolor", 0, t_keyboard, f_color_bkeys);
    CLASS_ATTR_LABEL                (c, "bkeycolor", 0, "Black Key Color");
    CLASS_ATTR_ORDER                (c, "bkeycolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bkeycolor", 0, "0. 0. 0. 0.");
    CLASS_ATTR_STYLE                (c, "bkeycolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "skeycolor", 0, t_keyboard, f_color_skeys);
    CLASS_ATTR_LABEL                (c, "skeycolor", 0, "Selected Key Color");
    CLASS_ATTR_ORDER                (c, "skeycolor", 0, "4");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "skeycolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "skeycolor", 0, "color");
    
    CLASS_ATTR_SYMBOL               (c, "mode", 0, t_keyboard, f_mode);
    CLASS_ATTR_LABEL                (c, "mode", 0, "Mode");
    CLASS_ATTR_ACCESSORS            (c, "mode", NULL, keyboard_mode_set);
    CLASS_ATTR_ORDER                (c, "mode", 0, "6");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "mode", 0, "monophonic");
    CLASS_ATTR_ITEMS                (c, "mode", 0, "monophonic polyphonic touchscreen");
    CLASS_ATTR_STYLE                (c, "mode", 0, "menu");
    
    CLASS_ATTR_LONG                 (c, "lowkey", 0, t_keyboard, f_low_key);
    CLASS_ATTR_LABEL                (c, "lowkey", 0, "Low Midi Key Value");
    CLASS_ATTR_ACCESSORS            (c, "lowkey", NULL, keyboard_lowkey_set);
    CLASS_ATTR_ORDER                (c, "lowkey", 0, "6");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "lowkey", 0, "36");
    CLASS_ATTR_STYLE                (c, "lowkey", 0, "number");
    
    CLASS_ATTR_LONG                 (c, "highkey", 0, t_keyboard, f_high_key);
    CLASS_ATTR_LABEL                (c, "highkey", 0, "High Midi Key Value");
    CLASS_ATTR_ACCESSORS            (c, "highkey", NULL, keyboard_highkey_set);
    CLASS_ATTR_ORDER                (c, "highkey", 0, "7");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "highkey", 0, "84");
    CLASS_ATTR_STYLE                (c, "highkey", 0, "number");
    
    eclass_register(CLASS_BOX, c);
    keyboard_class = c;
}







