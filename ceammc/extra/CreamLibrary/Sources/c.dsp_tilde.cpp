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

#if (_MSC_VER >= 1800)
static double round(double val)
{
	return floor(val + 0.5);
}
#endif


typedef struct  _dsp_tilde
{
	t_ebox      j_box;
    char        f_state;
    char        f_init;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_logo;
    
} t_dsp_tilde;

t_eclass *dsp_tildeclass;

static void *dsp_tilde_new(t_symbol *s, int argc, t_atom *argv)
{
    long flags;
	t_dsp_tilde *x  = (t_dsp_tilde *)eobj_new(dsp_tildeclass);
    t_binbuf* d     = binbuf_via_atoms(argc,argv);

    if(x && d)
    {
        flags = 0
        | EBOX_GROWLINK
        ;
        
        ebox_new((t_ebox *)x, flags);
        eobj_proxynew(x);
        x->f_init = 0;
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
        pd_bind((t_pd *)x, gensym("pd"));
        return x;
    }

	return NULL;
}

static void dsp_tilde_getdrawparams(t_dsp_tilde *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2.;
	params->d_cornersize        = 2.;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

static void dsp_tilde_oksize(t_dsp_tilde *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 30.);
    newrect->height = pd_clip_min(newrect->height, 30.);
}

static void dsp_tilde_free(t_dsp_tilde *x)
{
    pd_unbind((t_pd *)x, gensym("pd"));
	ebox_free((t_ebox *)x);
}

static void draw_background(t_dsp_tilde *x,  t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect->width, rect->height);
 
	if (g)
	{
        if(x->f_state)
        {
            egraphics_set_color_rgba(g, &x->f_color_logo);
        }
        else
        {
            egraphics_set_color_rgba(g, &x->f_color_border);
        }
        
        egraphics_circle(g, round(rect->width * 0.5f - 0.5f), round(rect->width * 0.5f - 0.5f), round(rect->width * 0.15f - 0.5f));
        egraphics_fill(g);
        
        egraphics_set_line_width(g, 2.);
        egraphics_arc(g, round(rect->width * 0.5f - 0.5f), round(rect->width * 0.5f - 0.5f), round(rect->width * 0.25f - 0.5f), EPD_PI, EPD_2PI);
        egraphics_stroke(g);
        
        egraphics_arc(g, round(rect->width * 0.5f - 0.5f), round(rect->width * 0.5f - 0.5f), round(rect->width * 0.35f - 0.5f), EPD_PI, EPD_2PI);
        egraphics_stroke(g);
        
		ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
	}
	ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0.f, 0.f);
}

static void dsp_tilde_paint(t_dsp_tilde *x, t_object *view)
{
    t_rect rect;
    if(!x->f_init)
    {
        x->f_state = canvas_dspstate;
        x->f_init = 1;
    }
    
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

static void dsp_tilde_anything(t_dsp_tilde *x, t_symbol* s, int argc, t_atom *argv)
{
    if(s == gensym("dsp") && argc && atom_gettype(argv) == A_FLOAT)
    {
        x->f_state = atom_getfloat(argv);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void dsp_tilde_open(t_dsp_tilde *x)
{
    sys_gui((char *)"pdsend \"pd audio-properties\"\n");
}

static void dsp_tilde_start(t_dsp_tilde *x)
{
    t_atom av;
    atom_setfloat(&av, 1);
    pd_typedmess((t_pd *)gensym("pd")->s_thing, gensym("dsp"), 1, &av);
    x->f_state = 1;
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}


static void dsp_tilde_stop(t_dsp_tilde *x)
{
    t_atom av;
    atom_setfloat(&av, 0);
    pd_typedmess((t_pd *)gensym("pd")->s_thing, gensym("dsp"), 1, &av);
    x->f_state = 0;
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

static void dsp_tilde_mousedown(t_dsp_tilde *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if(canvas_dspstate)
    {
        dsp_tilde_stop(x);
    }
    else
    {
        dsp_tilde_start(x);
    }
}

extern "C" void setup_c0x2edsp_tilde(void)
{
    t_eclass *c;
    
    c = eclass_new("c.dsp~", (method)dsp_tilde_new, (method)dsp_tilde_free, (short)sizeof(t_dsp_tilde), CLASS_NOINLET, A_GIMME, 0);
    
    eclass_guiinit(c, 0);

    eclass_addmethod(c, (method) dsp_tilde_paint,           "paint",            A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_getdrawparams,   "getdrawparams",    A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_oksize,          "oksize",           A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_mousedown,       "mousedown",        A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_anything,        "anything",         A_GIMME, 0);
    eclass_addmethod(c, (method) dsp_tilde_open,            "settings",         A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_start,           "start",            A_NULL, 0);
    eclass_addmethod(c, (method) dsp_tilde_stop,            "stop",             A_NULL, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
    CLASS_ATTR_DEFAULT              (c, "size", 0, "30 30");
    
    CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_dsp_tilde, f_color_background);
    CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_dsp_tilde, f_color_border);
    CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
    CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "logocolor", 0, t_dsp_tilde, f_color_logo);
    CLASS_ATTR_LABEL                (c, "logocolor", 0, "Logo Color");
    CLASS_ATTR_ORDER                (c, "logocolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "logocolor", 0, "0. 0.6 0. 0.8");
    CLASS_ATTR_STYLE                (c, "logocolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
    dsp_tildeclass = c;
}


