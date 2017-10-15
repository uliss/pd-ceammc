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

typedef struct _knob
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_needle;
    long        f_endless;
    long        f_mode;
    float       f_value;
    float       f_min;
    float       f_max;
    float       f_ref_y;
    float       f_ref_value;
} t_knob;

static t_eclass *knob_class;

static void knob_output(t_knob *x)
{
    t_pd* send = ebox_getsender((t_ebox *) x);
    outlet_float((t_outlet*)x->f_out, x->f_value);
    if(send)
    {
        pd_float(send, x->f_value);
    }
}

static void knob_set(t_knob *x, float f)
{
    if(x->f_endless)
    {
        x->f_value = (x->f_min < x->f_max) ? fmodf(f + x->f_max - x->f_min, x->f_max - x->f_min) : fmodf(f + x->f_min - x->f_max, x->f_min - x->f_max);
    }
    else
    {
        x->f_value = (x->f_min < x->f_max) ? pd_clip_minmax(f, x->f_min, x->f_max) : pd_clip_minmax(f, x->f_max, x->f_min);
    }
    
    ebox_invalidate_layer((t_ebox *)x, cream_sym_needle_layer);
    ebox_redraw((t_ebox *)x);
}

static void knob_float(t_knob *x, float f)
{
    knob_set(x, f);
    knob_output(x);
}

static t_pd_err knob_notify(t_knob *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if(msg == cream_sym_attr_modified)
	{
		if(s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_necolor || s == cream_sym_endless)
		{
			ebox_invalidate_layer((t_ebox *)x, cream_sym_needle_layer);
            ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
		}
	}
	return 0;
}

static void knob_getdrawparams(t_knob *x, t_object *patcherview, t_edrawparams *params)
{
    params->d_borderthickness   = 2;
    params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

static void knob_oksize(t_knob *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 25.);
    newrect->height = pd_clip_min(newrect->height, 25.);
}

static void draw_background(t_knob *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect->width, rect->height);
	if (g)
	{
        const float size = rect->width * 0.5f;
        
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_rectangle(g, 0.f, 0.f, rect->width, rect->height);
        egraphics_fill(g);
        
        egraphics_set_color_rgba(g, &x->f_color_background);
        egraphics_circle(g, rect->width * 0.5, rect->height * 0.5, size * 0.9);
        egraphics_fill(g);
        
        if(!x->f_endless)
        {
            const float hsize = pd_clip_min(size * 0.15f, 2.f);
            const float alpha = rect->height - hsize;
            egraphics_set_color_rgba(g, &x->f_color_border);
            
            egraphics_move_to(g, rect->width * 0.5f, rect->height * 0.5f + hsize);
            egraphics_line_to(g, rect->height * 0.5f - alpha * 0.5f, rect->height);
            egraphics_line_to(g, rect->height * 0.5f + alpha * 0.5f, rect->height);
            egraphics_fill(g);
            egraphics_set_color_rgba(g, &x->f_color_background);
            egraphics_circle(g, rect->width * 0.5f, rect->height * 0.5f, pd_clip_min(size * 0.3f, 3.f));
            egraphics_fill(g);
        }
        
        egraphics_set_color_rgba(g, &x->f_color_needle);
        egraphics_circle(g, rect->width * 0.5f, rect->height * 0.5f, pd_clip_min(size * 0.2f, 2.f));
        egraphics_fill(g);
        
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
	}
	ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0., 0.);
}

static void draw_needle(t_knob *x, t_object *view, t_rect *rect)
{
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_needle_layer, rect->width, rect->height);
    
    if(g)
	{
        egraphics_set_line_width(g, 2);
        egraphics_set_color_rgba(g, &x->f_color_needle);
        const float size = rect->width * 0.5f;
        
        if(x->f_endless)
        {
            const float value = (x->f_min < x->f_max) ? fmodf(x->f_value + x->f_max - x->f_min, x->f_max - x->f_min) : fmodf(x->f_value + x->f_min - x->f_max, x->f_min - x->f_max);
            const float pimul = EPD_2PI;
            const float pimin = EPD_PI2;
            if(x->f_min < x->f_max)
            {
                const float abs =  pd_abscissa(size * 0.9f, (value - x->f_min) / (x->f_max - x->f_min) * pimul + pimin);
                const float ord =  pd_ordinate(size * 0.9f, (value - x->f_min) / (x->f_max - x->f_min) * pimul + pimin);
                egraphics_line_fast(g, size, size, abs + size, ord + size);
                
            }
            else
            {
                const float abs =  pd_abscissa(size * 0.9f, -(value - x->f_max) / (x->f_min - x->f_max) * pimul + pimin);
                const float ord =  pd_ordinate(size * 0.9f, -(value - x->f_max) / (x->f_min - x->f_max) * pimul + pimin);
                egraphics_line_fast(g, size, size, abs + size, ord + size);
            }
            
        }
        else
        {
            const float pimul = 1.5f * EPD_PI;
            if(x->f_min < x->f_max)
            {
                const float pimin = 1.5 * EPD_PI2;
                const float value = pd_clip_minmax(x->f_value, x->f_min, x->f_max);
                const float abs =  pd_abscissa(size * 0.9 - 1, (value - x->f_min) / (x->f_max - x->f_min) * pimul + pimin);
                const float ord =  pd_ordinate(size * 0.9 - 1, (value - x->f_min) / (x->f_max - x->f_min) * pimul + pimin);
                egraphics_line_fast(g, size, size, abs + size, ord + size);
            }
            else
            {
                const float pimin = 0.5 * EPD_PI2;
                const float value = pd_clip_minmax(x->f_value, x->f_max, x->f_min);
                const float abs =  pd_abscissa(size * 0.9 - 1, -(value - x->f_max) / (x->f_min - x->f_max) * pimul + pimin);
                const float ord =  pd_ordinate(size * 0.9 - 1, -(value - x->f_max) / (x->f_min - x->f_max) * pimul + pimin);
                egraphics_line_fast(g, size, size, abs + size, ord + size);
            }
        }
        ebox_end_layer((t_ebox*)x, cream_sym_needle_layer);
    }
   
    ebox_paint_layer((t_ebox *)x, cream_sym_needle_layer, 0., 0.);

}

static void knob_paint(t_knob *x, t_object *view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    draw_background(x, view, &rect);
    draw_needle(x, view, &rect);
}

static void knob_mousedown(t_knob *x, t_object *patcherview, t_pt pt, long modifiers)
{
    float angle = pd_angle(pt.x - x->j_box.b_rect.width * 0.5, (x->j_box.b_rect.height * 0.5 - pt.y)) / EPD_2PI;
    if(x->f_mode)
    {
        if(x->f_endless)
        {
            if(x->f_min < x->f_max)
            {
                angle = -angle;
                angle -= 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                x->f_value = angle * (x->f_max - x->f_min) + x->f_min;
            }
            else
            {
                angle += 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                x->f_value = angle * (x->f_min - x->f_max) + x->f_max;
            }
        }
        else
        {
            if(x->f_min < x->f_max)
            {
                angle = -angle;
                angle -= 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                angle = pd_clip_minmax(angle, 0.125, 0.875);
                angle -= 0.125;
                angle *= 0.75;
                x->f_value = angle * (x->f_max - x->f_min) + x->f_min;
            }
            else
            {
                angle += 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                angle = pd_clip_minmax(angle, 0.125, 0.875);
                angle -= 0.125;
                angle *= 0.75;
                x->f_value = angle * (x->f_min - x->f_max) + x->f_max;
            }
        }
        knob_output(x);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_needle_layer);
        ebox_redraw((t_ebox *)x);
    }
    else
    {
        x->f_ref_y = pt.y;
        x->f_ref_value = x->f_value;
    }
}

static void knob_mousedrag(t_knob *x, t_object *patcherview, t_pt pt, long modifiers)
{
    float angle = pd_angle(pt.x - x->j_box.b_rect.width * 0.5, (x->j_box.b_rect.height * 0.5 - pt.y)) / EPD_2PI;
    if(x->f_mode)
    {
        if(x->f_endless)
        {
            if(x->f_min < x->f_max)
            {
                angle = -angle;
                angle -= 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                x->f_value = angle * (x->f_max - x->f_min) + x->f_min;
            }
            else
            {
                angle += 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                x->f_value = angle * (x->f_min - x->f_max) + x->f_max;
            }
        }
        else
        {
            if(x->f_min < x->f_max)
            {
                angle = -angle;
                angle -= 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                angle = pd_clip_minmax(angle, 0.125, 0.875);
                angle -= 0.125;
                angle *= 1. / 0.75;
                x->f_value = angle * (x->f_max - x->f_min) + x->f_min;
            }
        
            else
            {
                angle += 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                angle = pd_clip_minmax(angle, 0.125, 0.875);
                angle -= 0.125;
                angle *= 1. / 0.75;
                x->f_value = angle * (x->f_min - x->f_max) + x->f_max;
            }
        }
    }
    else
    {
        float diff = x->f_ref_y - pt.y;
        if(diff == 0xffffffff)
            return;
        if(x->f_min < x->f_max)
        {
            if(x->f_endless)
            {
                x->f_value = fmodf(diff / 50. * (x->f_max - x->f_min) + x->f_ref_value + x->f_max - x->f_min, x->f_max - x->f_min);
            }
            else
                x->f_value = pd_clip_minmax(diff / 50. * (x->f_max - x->f_min) + x->f_ref_value, x->f_min, x->f_max);
        }
        else
        {
            if(x->f_endless)
            {
                x->f_value = fmodf(diff / 50. * (x->f_min - x->f_max) + x->f_ref_value + x->f_min - x->f_max, x->f_min - x->f_max);
            }
            else
                x->f_value = pd_clip_minmax(diff / 50. * (x->f_min - x->f_max) + x->f_ref_value, x->f_max, x->f_min);
        }
    }
    knob_output(x);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_needle_layer);
    ebox_redraw((t_ebox *)x);
}

static void knob_preset(t_knob *x, t_binbuf *b)
{
    binbuf_addv(b, (char *)"sf", &s_float, (float)x->f_value);
}

static void *knob_new(t_symbol *s, int argc, t_atom *argv)
{
    t_knob *x = (t_knob *)eobj_new(knob_class);
    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if(x && d)
    {
        ebox_new((t_ebox *)x, 0 | EBOX_GROWLINK);
        x->f_out = outlet_new((t_object *)x, &s_float);
        x->f_value = 0;
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
        
        return x;
    }
    
    return NULL;
}

extern "C" void setup_c0x2eknob(void)
{
    t_eclass *c = eclass_new("c.knob", (method)knob_new, (method)ebox_free, (short)sizeof(t_knob), 0L, A_GIMME, 0);
    
    if(c)
    {
        eclass_guiinit(c, 0);
        
        eclass_addmethod(c, (method) knob_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) knob_notify,          "notify",           A_NULL, 0);
        eclass_addmethod(c, (method) knob_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) knob_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) knob_set,             "set",              A_FLOAT,0);
        eclass_addmethod(c, (method) knob_float,           "float",            A_FLOAT,0);
        eclass_addmethod(c, (method) knob_output,          "bang",             A_NULL, 0);
        eclass_addmethod(c, (method) knob_mousedown,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) knob_mousedrag,       "mousedrag",        A_NULL, 0);
        eclass_addmethod(c, (method) knob_preset,          "preset",           A_NULL, 0);
        
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        CLASS_ATTR_DEFAULT              (c, "size", 0, "50. 50.");
        
        CLASS_ATTR_LONG                 (c, "endless", 0, t_knob, f_endless);
        CLASS_ATTR_LABEL                (c, "endless", 0, "Endless Mode");
        CLASS_ATTR_ORDER                (c, "endless", 0, "1");
        CLASS_ATTR_FILTER_CLIP          (c, "endless", 0, 1);
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "endless", 0, "0");
        CLASS_ATTR_STYLE                (c, "endless", 0, "onoff");
        
        CLASS_ATTR_LONG                 (c, "mode", 0, t_knob, f_mode);
        CLASS_ATTR_LABEL                (c, "mode", 0, "Circular Mode");
        CLASS_ATTR_ORDER                (c, "mode", 0, "1");
        CLASS_ATTR_FILTER_CLIP          (c, "mode", 0, 1);
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "mode", 0, "0");
        CLASS_ATTR_STYLE                (c, "mode", 0, "onoff");
        
        CLASS_ATTR_FLOAT                (c, "min", 0, t_knob, f_min);
        CLASS_ATTR_LABEL                (c, "min", 0, "Minimum Value");
        CLASS_ATTR_ORDER                (c, "min", 0, "1");
        CLASS_ATTR_DEFAULT              (c, "min", 0, "0.");
        CLASS_ATTR_SAVE                 (c, "min", 1);
        CLASS_ATTR_STYLE                (c, "min", 0, "number");
        
        CLASS_ATTR_FLOAT                (c, "max", 0, t_knob, f_max);
        CLASS_ATTR_LABEL                (c, "max", 0, "Maximum Value");
        CLASS_ATTR_ORDER                (c, "max", 0, "1");
        CLASS_ATTR_DEFAULT              (c, "max", 0, "1.");
        CLASS_ATTR_SAVE                 (c, "max", 1);
        CLASS_ATTR_STYLE                (c, "max", 0, "number");
        
        CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_knob, f_color_background);
        CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
        CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_knob, f_color_border);
        CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
        CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "necolor", 0, t_knob, f_color_needle);
        CLASS_ATTR_LABEL                (c, "necolor", 0, "Needle Color");
        CLASS_ATTR_ORDER                (c, "necolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "necolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "necolor", 0, "color");
        
        eclass_register(CLASS_BOX, c);
        knob_class = c;
    }
}



