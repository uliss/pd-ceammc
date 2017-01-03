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

typedef struct _slider
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_knob;
    char        f_direction;
    float       f_min;
    float       f_max;
    float       f_value;
    float       f_value_ref;
    float       f_value_last;
    long        f_mode;
} t_slider;

static t_eclass *slider_class;

static void slider_output(t_slider *x)
{
    t_pd* send = ebox_getsender((t_ebox *) x);
    outlet_float((t_outlet*)x->f_out, x->f_value);
    if(send)
    {
        pd_float(send, x->f_value);
    }
}

static void slider_float(t_slider *x, float f)
{
    if(x->f_min < x->f_max)
        x->f_value = pd_clip_minmax(f, x->f_min, x->f_max);
    else
        x->f_value = pd_clip_minmax(f, x->f_max, x->f_min);
    
    slider_output(x);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

static void slider_bang(t_slider *x)
{
    slider_output(x);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

static void slider_set(t_slider *x, float f)
{
    if(x->f_min < x->f_max)
        x->f_value = pd_clip_minmax(f, x->f_min, x->f_max);
    else
        x->f_value = pd_clip_minmax(f, x->f_max, x->f_min);
    
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

static void slider_getdrawparams(t_slider *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

static void slider_oksize(t_slider *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8.);
    newrect->height = pd_clip_min(newrect->height, 8.);
    x->f_direction = newrect->width > newrect->height ? 1 : 0;
    if(x->f_direction)
    {
        newrect->width = pd_clip_min(newrect->width, 50.);
    }
    else
    {
        newrect->height = pd_clip_min(newrect->height, 50.);
    }
}

static t_pd_err slider_notify(t_slider *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if(msg == cream_sym_attr_modified)
	{
		if(s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_kncolor)
		{
			ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
		}
	}
	return 0;
}

static void slider_paint(t_slider *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect.width, rect.height);
    
    if (g)
    {
        const float value = (x->f_value - x->f_min) / (x->f_max - x->f_min);
        egraphics_set_color_rgba(g, &x->f_color_knob);
        egraphics_set_line_width(g, 2);
        if(x->f_direction)
        {
            egraphics_line_fast(g, value * rect.width, 0, value * rect.width, rect.height);
        }
        else
        {
            egraphics_line_fast(g, 0, (1. - value) * rect.height, rect.width, (1. - value) * rect.height);
        }
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0., 0.);
}


static void slider_mousedown(t_slider *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    const float ratio = (x->f_min < x->f_max) ? x->f_max - x->f_min : x->f_min - x->f_max;
    if(x->f_mode)
    {
        x->f_value_last = x->f_value;
        if(x->f_direction)
        {
            if(x->f_min < x->f_max)
                x->f_value_ref = pd_clip_minmax(pt.x / rect.width * ratio + x->f_min, x->f_min, x->f_max);
            else
                x->f_value_ref = pd_clip_minmax((rect.width - pt.x) / rect.width * ratio + x->f_max, x->f_max, x->f_min);
        }
        else
        {
            if(x->f_min < x->f_max)
                x->f_value_ref = pd_clip_minmax((rect.height - pt.y) / rect.height * ratio + x->f_min, x->f_min, x->f_max);
            else
                x->f_value_ref = pd_clip_minmax(pt.y / rect.height * ratio + x->f_max, x->f_max, x->f_min);
        }
    }
    else
    {
        if(x->f_direction)
        {
            if(x->f_min < x->f_max)
                x->f_value = pd_clip_minmax(pt.x / rect.width * ratio + x->f_min, x->f_min, x->f_max);
            else
                x->f_value = pd_clip_minmax((rect.width - pt.x) / rect.width * ratio + x->f_max, x->f_max, x->f_min);
        }
        else
        {
            if(x->f_min < x->f_max)
                x->f_value = pd_clip_minmax((rect.height - pt.y) / rect.height * ratio + x->f_min, x->f_min, x->f_max);
            else
                x->f_value = pd_clip_minmax(pt.y / rect.height * ratio + x->f_max, x->f_max, x->f_min);
        }
        
        slider_output(x);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void slider_mousedrag(t_slider *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    const float ratio = (x->f_min < x->f_max) ? x->f_max - x->f_min : x->f_min - x->f_max;
    
    if(x->f_mode)
    {
        float newvalue;
        if(x->f_direction)
        {
            if(x->f_min < x->f_max)
                newvalue = pt.x / rect.width * ratio + x->f_min;
            else
                newvalue = (rect.width - pt.x) / rect.width * ratio + x->f_max;
        }
        else
        {
            if(x->f_min < x->f_max)
                newvalue = (rect.height - pt.y) / rect.height * ratio + x->f_min;
            else
                newvalue = pt.y / rect.height * ratio + x->f_max;
        }
        if(x->f_min < x->f_max)
            x->f_value = pd_clip_minmax(x->f_value_last + newvalue - x->f_value_ref, x->f_min, x->f_max);
        else
            x->f_value = pd_clip_minmax(x->f_value_last + newvalue - x->f_value_ref, x->f_max, x->f_min);
        
        if(x->f_value == x->f_min || x->f_value == x->f_max)
        {
            x->f_value_last = x->f_value;
            x->f_value_ref  = newvalue;
        }
    }
    else
    {
        if(x->f_direction)
        {
            if(x->f_min < x->f_max)
                x->f_value = pd_clip_minmax(pt.x / rect.width * ratio + x->f_min, x->f_min, x->f_max);
            else
                x->f_value = pd_clip_minmax((rect.width - pt.x) / rect.width * ratio + x->f_max, x->f_max, x->f_min);
        }
        else
        {
            if(x->f_min < x->f_max)
                x->f_value = pd_clip_minmax((rect.height - pt.y) / rect.height * ratio + x->f_min, x->f_min, x->f_max);
            else
                x->f_value = pd_clip_minmax(pt.y / rect.height * ratio + x->f_max, x->f_max, x->f_min);
        }
    }
    
    slider_output(x);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

static void slider_preset(t_slider *x, t_binbuf *b)
{
    binbuf_addv(b, (char *)"sf", &s_float, (float)x->f_value);
}

static void *slider_new(t_symbol *s, int argc, t_atom *argv)
{
    t_slider *x = (t_slider *)eobj_new(slider_class);
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    
    if(x && d)
    {
        ebox_new((t_ebox *)x, 0 | EBOX_GROWINDI);
        x->f_out = outlet_new((t_object *)x, &s_float);
        ebox_attrprocess_viabinbuf(x, d);
        x->f_value = x->f_min;
        ebox_ready((t_ebox *)x);
        return x;
    }
    
    return NULL;
}

extern "C" void setup_c0x2eslider(void)
{
    t_eclass *c = eclass_new("c.slider", (method)slider_new, (method)ebox_free, (short)sizeof(t_slider), 0L, A_GIMME, 0);
    if(c)
    {
        eclass_guiinit(c, 0);
        
        eclass_addmethod(c, (method) slider_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) slider_notify,          "notify",           A_NULL, 0);
        eclass_addmethod(c, (method) slider_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) slider_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) slider_set,             "set",              A_FLOAT,0);
        eclass_addmethod(c, (method) slider_float,           "float",            A_FLOAT,0);
        eclass_addmethod(c, (method) slider_bang,            "bang",             A_NULL, 0);
        eclass_addmethod(c, (method) slider_mousedown,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) slider_mousedrag,       "mousedrag",        A_NULL, 0);
        eclass_addmethod(c, (method) slider_preset,          "preset",           A_NULL, 0);
        
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 120.");
        
        CLASS_ATTR_LONG                 (c, "mode", 0, t_slider, f_mode);
        CLASS_ATTR_LABEL                (c, "mode", 0, "Relative Mode");
        CLASS_ATTR_ORDER                (c, "mode", 0, "1");
        CLASS_ATTR_FILTER_CLIP          (c, "mode", 0, 1);
        CLASS_ATTR_DEFAULT              (c, "mode", 0, "0");
        CLASS_ATTR_SAVE                 (c, "mode", 1);
        CLASS_ATTR_STYLE                (c, "mode", 0, "onoff");
        
        CLASS_ATTR_FLOAT                (c, "min", 0, t_slider, f_min);
        CLASS_ATTR_LABEL                (c, "min", 0, "Minimum Value");
        CLASS_ATTR_ORDER                (c, "min", 0, "1");
        CLASS_ATTR_DEFAULT              (c, "min", 0, "0.");
        CLASS_ATTR_SAVE                 (c, "min", 1);
        CLASS_ATTR_STYLE                (c, "min", 0, "number");
        
        CLASS_ATTR_FLOAT                (c, "max", 0, t_slider, f_max);
        CLASS_ATTR_LABEL                (c, "max", 0, "Maximum Value");
        CLASS_ATTR_ORDER                (c, "max", 0, "1");
        CLASS_ATTR_DEFAULT              (c, "max", 0, "1.");
        CLASS_ATTR_SAVE                 (c, "max", 1);
        CLASS_ATTR_STYLE                (c, "max", 0, "number");
        
        CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_slider, f_color_background);
        CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
        CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_slider, f_color_border);
        CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
        CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "kncolor", 0, t_slider, f_color_knob);
        CLASS_ATTR_LABEL                (c, "kncolor", 0, "Knob Color");
        CLASS_ATTR_ORDER                (c, "kncolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "kncolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "kncolor", 0, "color");
        
        eclass_register(CLASS_BOX, c);
        slider_class = c;
    }
}





