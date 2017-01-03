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

typedef struct _radio
{
	t_ebox      j_box;
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_item;
    char        f_direction;
    char        f_items[CREAM_MAXITEMS];
    int         f_nitems;
    int         f_mode;
} t_radio;

static t_eclass *radio_class;

static void radio_getdrawparams(t_radio *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

static void radio_oksize(t_radio *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
    x->f_direction = (newrect->width > newrect->height) ? 1 : 0;
    if(x->f_direction)
    {
        newrect->width = pd_clip_min(newrect->width, x->f_nitems * newrect->height);
    }
    else
    {
        newrect->height = pd_clip_min(newrect->height, x->f_nitems * newrect->width);
    }
}

static void radio_output(t_radio *x)
{
    int i;
    t_atom av[CREAM_MAXITEMS];
    t_pd* send = ebox_getsender((t_ebox *) x);
    if(x->f_mode)
    {
        for(i = 0; i < x->f_nitems; i++)
        {
            atom_setfloat(av+i, (float)x->f_items[i]);
        }
        outlet_list(x->f_out, &s_list, (int)x->f_nitems, av);
        if(send)
        {
            pd_list(send, &s_list, (int)x->f_nitems, av);
        }
    }
    else
    {
        for(i = 0; i < x->f_nitems; i++)
        {
            if(x->f_items[i] != 0)
            {
                outlet_float(x->f_out, (float)i);
                if(send)
                {
                    pd_float(send, (float)i);
                }
            }
        }
    }
}

static void radio_set_one(t_radio *x, int f)
{
    if(f >= 0 && f < x->f_nitems)
    {
        memset(x->f_items, 0, (size_t)x->f_nitems * sizeof(int));
        x->f_items[f] = 1;
    }
}

static void radio_set_list(t_radio *x, int argc, t_atom *argv)
{
    for(int i = 0; i < argc && i < x->f_nitems; i++)
    {
        x->f_items[i] = (atom_gettype(argv+i) == A_FLOAT && atom_getfloat(argv+i) == 0) ? 0 : 1;
    }
}

static void radio_set(t_radio *x, t_symbol* s, int argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(x->f_mode)
        {
            radio_set_list(x, argc, argv);
        }
        else if(atom_gettype(argv) == A_FLOAT)
        {
            radio_set_one(x, (int)atom_getfloat(argv));
        }
        
        ebox_invalidate_layer((t_ebox *)x, cream_sym_items_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void radio_float(t_radio *x, float f)
{
    if(x->f_mode)
    {
        t_atom av;
        atom_setfloat(&av, f);
        radio_set_list(x, 1, &av);
    }
    else
    {
        radio_set_one(x, (int)f);
    }
    
    radio_output(x);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_items_layer);
    ebox_redraw((t_ebox *)x);
}

static void radio_list(t_radio *x, t_symbol* s, int argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(x->f_mode)
        {
            radio_set_list(x, argc, argv);
        }
        else if(atom_gettype(argv) == A_FLOAT)
        {
            radio_set_one(x, (int)atom_getfloat(argv));
        }
        
        radio_output(x);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_items_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static t_pd_err radio_notify(t_radio *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if(msg == cream_sym_attr_modified)
	{
		if(s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_itcolor)
		{
			ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
            ebox_invalidate_layer((t_ebox *)x, cream_sym_items_layer);
		}
	}
	return 0;
}

static void draw_background(t_radio *x, t_object *view, t_rect *rect)
{
	int i;
	t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect->width, rect->height);
    
	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 1);
        egraphics_rectangle(g, 0, 0, rect->width, rect->height);
        egraphics_fill(g);
        egraphics_set_color_rgba(g, &x->f_color_background);
        const float ratio = x->f_direction ? (rect->width / x->f_nitems) : rect->height / x->f_nitems;
        if(x->f_mode)
        {
            const float offset = x->f_direction ? (rect->height * 0.45) : rect->width * 0.45;
            if(x->f_direction)
            {
                for(i = 0; i < x->f_nitems; i++)
                {
                    egraphics_rectangle(g, (i + 0.5) * ratio - offset, rect->height * 0.05, offset * 2., offset * 2.);
                    egraphics_fill(g);
                }
            }
            else
            {
                for(i = 0; i < x->f_nitems; i++)
                {
                    egraphics_rectangle(g, rect->width * 0.05, (i + 0.5) * ratio - offset, offset * 2., offset * 2.);
                    egraphics_fill(g);
                }
            }
        }
        else
        {
            if(x->f_direction)
            {
                for(i = 0; i < x->f_nitems; i++)
                {
                    egraphics_circle(g, (i + 0.5) * ratio, rect->height * 0.5, rect->height * 0.45);
                    egraphics_fill(g);
                }
            }
            else
            {
                for(i = 0; i < x->f_nitems; i++)
                {
                    egraphics_circle(g, rect->width * 0.5, (i + 0.5) * ratio, rect->width * 0.45);
                    egraphics_fill(g);
                }
            }
        }
        
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
	}
	ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0., 0.);
}

static void draw_items(t_radio *x, t_object *view, t_rect *rect)
{
	int i;
	t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_items_layer, rect->width, rect->height);
    
	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_item);
        const float ratio = x->f_direction ? (rect->width / x->f_nitems) : rect->height / x->f_nitems;
        if(x->f_mode)
        {
            const float offset = x->f_direction ? (rect->height * 0.35) : rect->width * 0.35;
            if(x->f_direction)
            {
                for(i = 0; i < x->f_nitems; i++)
                {
                    if(x->f_items[i])
                    {
                        egraphics_rectangle(g, (i + 0.5) * ratio - offset, rect->height * 0.15, offset * 2., offset * 2.);
                        egraphics_fill(g);
                    }
                }
            }
            else
            {
                for(i = 0; i < x->f_nitems; i++)
                {
                    if(x->f_items[i])
                    {
                        egraphics_rectangle(g, rect->width * 0.15, (i + 0.5) * ratio - offset, offset * 2., offset * 2.);
                        egraphics_fill(g);
                    }
                }
            }
        }
        else
        {
            if(x->f_direction)
            {
                for(i = 0; i < x->f_nitems; i++)
                {
                    if(x->f_items[i])
                    {
                        egraphics_circle(g, (i + 0.5) * ratio, rect->height * 0.5, rect->height * 0.35);
                        egraphics_fill(g);
                    }
                }
            }
            else
            {
                for(i = 0; i < x->f_nitems; i++)
                {
                    if(x->f_items[i])
                    {
                        egraphics_circle(g, rect->width * 0.5, (i + 0.5) * ratio, rect->width * 0.35);
                        egraphics_fill(g);
                    }
                }
            }
        }
        
        ebox_end_layer((t_ebox*)x, cream_sym_items_layer);
	}
	ebox_paint_layer((t_ebox *)x, cream_sym_items_layer, 0., 0.);
}

static void radio_paint(t_radio *x, t_object *view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
    draw_items(x, view, &rect);
}

static void radio_mousedown(t_radio *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int i;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    const int index = x->f_direction ? (pt.x / rect.width * x->f_nitems) : (pt.y / rect.height * x->f_nitems);
    if(index >= 0 && index < x->f_nitems)
    {
        if(x->f_mode)
        {
            if(x->f_items[index] == 0)
                x->f_items[index] = 1;
            else
                x->f_items[index] = 0;
        }
        else
        {
            for(i = 0; i < x->f_nitems; i++)
                x->f_items[i] = 0;
            x->f_items[index] = 1;
        }
        
        radio_output(x);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_items_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void radio_preset(t_radio *x, t_binbuf *b)
{
    int i;
    if(x->f_mode)
    {
        binbuf_addv(b, (char *)"s", &s_list);
        for(i = 0; i < x->f_nitems; i++)
        {
            binbuf_addv(b, (char *)"f", (float)x->f_items[i]);
        }

    }
    else
    {
        for(i = 0; i < x->f_nitems; i++)
        {
            if(x->f_items[i])
            {
                binbuf_addv(b, (char *)"sf", &s_float, (float)i);
            }
        }
    }
}

static t_pd_err radio_nitems_set(t_radio *x, t_object *attr, int ac, t_atom *av)
{
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        const int nitems = pd_clip_minmax(atom_getfloat(av), 1, CREAM_MAXITEMS);
        if(nitems != x->f_nitems)
        {
            t_rect rect;
            ebox_get_rect_for_view((t_ebox *)x, &rect);
            x->f_nitems = nitems;
            memset(x->f_items, 0, (size_t)x->f_nitems * sizeof(int));
            ebox_notify((t_ebox *)x, s_size, cream_sym_attr_modified, NULL, NULL);
        }
    }
    return 0;
}

static t_pd_err radio_mode_set(t_radio *x, t_object *attr, int ac, t_atom *av)
{
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        const char mode = atom_getfloat(av) == 0 ? 0 : 1;
        if(mode != x->f_mode)
        {
            x->f_mode = mode;
            memset(x->f_items, 0, (size_t)x->f_nitems * sizeof(int));
            ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
            ebox_invalidate_layer((t_ebox *)x, cream_sym_items_layer);
            ebox_redraw((t_ebox *)x);
        }
    }
    return 0;
}

static void *radio_new(t_symbol *s, int argc, t_atom *argv)
{
    t_radio *x = (t_radio *)eobj_new(radio_class);
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    
    if(x && d)
    {
        ebox_new((t_ebox *)x, 0 | EBOX_GROWINDI);
        x->f_out = outlet_new((t_object *)x, &s_anything);
        binbuf_get_attribute_int(d, gensym("nitems"), &x->f_nitems);
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
        
        return x;
    }
    return NULL;
}

extern "C" void setup_c0x2eradio(void)
{
    t_eclass *c = eclass_new("c.radio", (method)radio_new, (method)ebox_free, (short)sizeof(t_radio), 0L, A_GIMME, 0);
    if(c)
    {
        eclass_guiinit(c, 0);
        eclass_addmethod(c, (method) radio_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) radio_notify,          "notify",           A_NULL, 0);
        eclass_addmethod(c, (method) radio_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) radio_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) radio_set,             "set",              A_GIMME,0);
        eclass_addmethod(c, (method) radio_list,            "list",             A_GIMME,0);
        eclass_addmethod(c, (method) radio_float,           "float",            A_FLOAT,0);
        eclass_addmethod(c, (method) radio_output,          "bang",             A_NULL, 0);
        eclass_addmethod(c, (method) radio_mousedown,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) radio_preset,          "preset",           A_NULL, 0);
        
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 120.");
        
        CLASS_ATTR_INT                  (c, "nitems", 0, t_radio, f_nitems);
        CLASS_ATTR_LABEL                (c, "nitems", 0, "Number of Items");
        CLASS_ATTR_ACCESSORS			(c, "nitems", NULL, radio_nitems_set);
        CLASS_ATTR_ORDER                (c, "nitems", 0, "1");
        CLASS_ATTR_FILTER_CLIP          (c, "nitems", 1, CREAM_MAXITEMS);
        CLASS_ATTR_DEFAULT              (c, "nitems", 0, "8");
        CLASS_ATTR_SAVE                 (c, "nitems", 1);
        CLASS_ATTR_STYLE                (c, "nitems", 0, "number");
        
        CLASS_ATTR_INT                  (c, "mode", 0, t_radio, f_mode);
        CLASS_ATTR_LABEL                (c, "mode", 0, "Check List Mode");
        CLASS_ATTR_ACCESSORS			(c, "mode", NULL, radio_mode_set);
        CLASS_ATTR_ORDER                (c, "mode", 0, "1");
        CLASS_ATTR_FILTER_CLIP          (c, "mode", 0, 1);
        CLASS_ATTR_DEFAULT              (c, "mode", 0, "0");
        CLASS_ATTR_SAVE                 (c, "mode", 1);
        CLASS_ATTR_STYLE                (c, "mode", 0, "onoff");
        
        CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_radio, f_color_background);
        CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
        CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_radio, f_color_border);
        CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
        CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "itcolor", 0, t_radio, f_color_item);
        CLASS_ATTR_LABEL                (c, "itcolor", 0, "Item Color");
        CLASS_ATTR_ORDER                (c, "itcolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "itcolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "itcolor", 0, "color");
        
        eclass_register(CLASS_BOX, c);
        radio_class = c;
    }
}




