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

typedef struct _rslider
{
	t_ebox      j_box;
    t_outlet*   f_out_left;
    t_outlet*   f_out_right;
    long        f_mode;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_knob;
    char        f_direction;
    char        f_loworhigh;
    float       f_min;
    float       f_max;
    float       f_value_low;
    float       f_value_high;
} t_rslider;

static t_eclass *rslider_class;

static void *rslider_new(t_symbol *s, int argc, t_atom *argv)
{
    t_rslider *x = (t_rslider *)eobj_new(rslider_class);
    t_binbuf  *d = binbuf_via_atoms(argc,argv);
    
    if(x && d)
    {
        ebox_new((t_ebox *)x, 0 | EBOX_GROWINDI);
        eobj_proxynew(x);
        eobj_proxynew(x);
        x->f_out_left = outlet_new((t_object *)x, &s_list);
        x->f_out_right = outlet_new((t_object *)x, &s_float);
        
        ebox_attrprocess_viabinbuf(x, d);
        x->f_value_low = x->f_min;
        ebox_ready((t_ebox *)x);
        
        return x;
    }
    
    return NULL;
}

static void rslider_output(t_rslider *x)
{
    t_atom argv[2];
    atom_setfloat(argv, x->f_value_low);
    atom_setfloat(argv+1, x->f_value_high);
    if(x->f_mode)
    {
        outlet_list(x->f_out_left, &s_list, 2, argv);
    }
    else
    {
        outlet_float(x->f_out_left, (float)x->f_value_low);
        outlet_float(x->f_out_right, (float)x->f_value_high);
    }
    if(ebox_getsender((t_ebox *) x))
    {
        pd_list(ebox_getsender((t_ebox *) x), &s_list, 2, argv);
    }
}

static void rslider_set(t_rslider *x, t_symbol* s, int argc, t_atom *argv)
{
    if(argc > 0 && atom_gettype(argv) == A_FLOAT)
    {
        if(x->f_min < x->f_max)
        {
            x->f_value_low = pd_clip_minmax(atom_getfloat(argv), x->f_min, x->f_max);
        }
        else
        {
            x->f_value_low = pd_clip_minmax(atom_getfloat(argv), x->f_max, x->f_min);
        }
    }
    if(argc > 1 && atom_gettype(argv+1) == A_FLOAT)
    {
        if(x->f_min < x->f_max)
        {
            x->f_value_high = pd_clip_minmax(atom_getfloat(argv+1), x->f_min, x->f_max);
        }
        else
        {
            x->f_value_high = pd_clip_minmax(atom_getfloat(argv+1), x->f_max, x->f_min);
        }
    }
    
    ebox_invalidate_layer((t_ebox *)x, cream_sym_knob_layer);
    ebox_redraw((t_ebox *)x);
}


static void rslider_bang(t_rslider *x)
{
    ebox_invalidate_layer((t_ebox *)x, cream_sym_knob_layer);
    ebox_redraw((t_ebox *)x);
    rslider_output(x);
}

static void rslider_float(t_rslider *x, float f)
{
    if(eobj_getproxy(x) == 0)
    {
        if(x->f_min < x->f_max)
        {
            x->f_value_low = pd_clip_minmax(f, x->f_min, x->f_max);
        }
        else
        {
            x->f_value_low = pd_clip_minmax(f, x->f_max, x->f_min);
        }
    }
    else
    {
        if(x->f_min < x->f_max)
        {
            x->f_value_high = pd_clip_minmax(f, x->f_min, x->f_max);
        }
        else
        {
            x->f_value_high = pd_clip_minmax(f, x->f_max, x->f_min);
        }
    }
    rslider_output(x);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_knob_layer);
    ebox_redraw((t_ebox *)x);
}

static void rslider_list(t_rslider *x, t_symbol* s, int argc, t_atom *argv)
{
    rslider_set(x, s, argc, argv);
    rslider_output(x);
}

static void rslider_preset(t_rslider *x, t_binbuf *b)
{
    binbuf_addv(b, (char *)"sff", gensym("list"), (float)x->f_value_low, (float)x->f_value_high);
}

static void rslider_getdrawparams(t_rslider *x, t_object *patcherview, t_edrawparams *params)
{
    params->d_borderthickness   = 2;
    params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

static void rslider_oksize(t_rslider *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 16.);
    newrect->height = pd_clip_min(newrect->height, 16.);
    
    if(newrect->width > newrect->height)
        x->f_direction = 1;
    else
        x->f_direction = 0;
    
    if(x->f_direction)
    {
        newrect->width = pd_clip_min(newrect->width, 50.);
        if((int)newrect->height % 2 == 1)
            newrect->height++;
    }
    else
    {
        newrect->height = pd_clip_min(newrect->height, 50.);
        if((int)newrect->width % 2 == 1)
            newrect->width++;
    }
}

static void draw_background(t_rslider *x, t_object *view, t_rect *rect)
{
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect->width, rect->height);
    
    if (g)
    {
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2);
        if(x->f_direction)
        {
            egraphics_line_fast(g,  -2, rect->height * 0.5, rect->width + 4, rect->height * 0.5);
        }
        else
        {
            egraphics_line_fast(g, rect->width * 0.5, -2, rect->width * 0.5, rect->height  + 4);
        }
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0., 0.);
}

static void draw_knob(t_rslider *x, t_object *view, t_rect *rect)
{
    t_rgba linecolor;
    t_rgba rectcolor;
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_knob_layer, rect->width, rect->height);
    
    if (g)
    {
        float value_low = (x->f_value_low - x->f_min) / (x->f_max - x->f_min);
        float value_high = (x->f_value_high - x->f_min) / (x->f_max - x->f_min);
        rgba_set(&rectcolor, (x->f_color_background.red + x->f_color_knob.red) * 0.5, (x->f_color_background.green + x->f_color_knob.green) * 0.5, (x->f_color_background.blue + x->f_color_knob.blue) * 0.5, 1.);
        
        rgba_set(&linecolor, (x->f_color_border.red + x->f_color_knob.red) * 0.5, (x->f_color_border.green + x->f_color_border.green) * 0.5, (x->f_color_border.blue + x->f_color_knob.blue) * 0.5, 1.);
        
        egraphics_set_line_width(g, 2);
        if(x->f_direction)
        {
            egraphics_set_color_rgba(g, &rectcolor);
            egraphics_rectangle(g, value_low * rect->width, -2, value_high * rect->width - value_low * rect->width, rect->height + 4);
            egraphics_fill(g);
            
            egraphics_set_color_rgba(g, &linecolor);
            egraphics_line_fast(g,  value_low * rect->width, rect->height * 0.5, value_high * rect->width, rect->height * 0.5);
            
            egraphics_set_color_rgba(g, &x->f_color_knob);
            egraphics_line_fast(g, value_low * rect->width, -2, value_low * rect->width, rect->height + 4);
            egraphics_line_fast(g, value_high * rect->width, -2, value_high * rect->width, rect->height + 4);
        }
        else
        {
            egraphics_set_color_rgba(g, &rectcolor);
            egraphics_rectangle(g, -2, value_low * rect->height, rect->width + 4, value_high * rect->height - value_low * rect->height);
            egraphics_fill(g);
            
            egraphics_set_color_rgba(g, &linecolor);
            egraphics_line_fast(g, rect->width * 0.5, value_low * rect->height, rect->width * 0.5, value_high * rect->height);
            
            egraphics_set_color_rgba(g, &x->f_color_knob);
            egraphics_line_fast(g, -2, value_low * rect->height, rect->width + 4, value_low * rect->height);
            egraphics_line_fast(g, -2, value_high * rect->height, rect->width + 4, value_high * rect->height);
        }
        ebox_end_layer((t_ebox*)x, cream_sym_knob_layer);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_knob_layer, 0., 0.);
}

static void rslider_paint(t_rslider *x, t_object *view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
    draw_knob(x, view, &rect);
}

static void rslider_mousedown(t_rslider *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    float ratio;
    float value;
    if(x->f_min < x->f_max)
        ratio = x->f_max - x->f_min;
    else
        ratio = x->f_min - x->f_max;
    
    if(x->f_direction)
    {
        if(x->f_min < x->f_max)
            value = pd_clip_minmax(pt.x / rect.width * ratio + x->f_min, x->f_min, x->f_max);
        else
            value= pd_clip_minmax((rect.width - pt.x) / rect.width * ratio + x->f_max, x->f_max, x->f_min);
    }
    else
    {
        if(x->f_min < x->f_max)
            value = pd_clip_minmax(pt.y / rect.height * ratio + x->f_min, x->f_min, x->f_max);
        else
            value = pd_clip_minmax((rect.height - pt.y) / rect.height * ratio + x->f_max, x->f_max, x->f_min);
    }
    
    if(modifiers == EMOD_SHIFT)
    {
        if(fabs(x->f_value_high - value) < fabs(x->f_value_low - value))
        {
            x->f_value_high = value;
            x->f_loworhigh = 0;
        }
        else
        {
            x->f_value_low = value;
            x->f_loworhigh = 1;
        }
    }
    else
    {
        x->f_value_high = x->f_value_low = value;
        x->f_loworhigh = 0;
    }
    
    rslider_output(x);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_knob_layer);
    ebox_redraw((t_ebox *)x);
}

static void rslider_mousedrag(t_rslider *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    float ratio;
    float value;
    if(x->f_min < x->f_max)
        ratio = x->f_max - x->f_min;
    else
        ratio = x->f_min - x->f_max;
    
    if(x->f_direction)
    {
        if(x->f_min < x->f_max)
            value = pd_clip_minmax(pt.x / rect.width * ratio + x->f_min, x->f_min, x->f_max);
        else
            value = pd_clip_minmax((rect.width - pt.x) / rect.width * ratio + x->f_max, x->f_max, x->f_min);
    }
    else
    {
        if(x->f_min < x->f_max)
            value = pd_clip_minmax(pt.y / rect.height * ratio + x->f_min, x->f_min, x->f_max);
        else
            value = pd_clip_minmax((rect.height - pt.y) / rect.height * ratio + x->f_max, x->f_max, x->f_min);
    }
    
    if(x->f_loworhigh)
    {
        x->f_value_low = value;
    }
    else
    {
        x->f_value_high = value;
    }
    
    rslider_output(x);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_knob_layer);
    ebox_redraw((t_ebox *)x);
}

extern "C" void setup_c0x2erslider(void)
{
	t_eclass *c = eclass_new("c.rslider", (method)rslider_new, (method)ebox_free, (short)sizeof(t_rslider), CLASS_NOINLET, A_GIMME, 0);
    
    if(c)
    {
        eclass_guiinit(c, 0);
        
        eclass_addmethod(c, (method) rslider_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) rslider_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) rslider_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) rslider_set,             "set",              A_GIMME,0);
        eclass_addmethod(c, (method) rslider_list,            "list",             A_GIMME,0);
        eclass_addmethod(c, (method) rslider_float,           "float",            A_FLOAT,0);
        eclass_addmethod(c, (method) rslider_bang,            "bang",             A_NULL, 0);
        eclass_addmethod(c, (method) rslider_mousedown,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) rslider_mousedrag,       "mousedrag",        A_NULL, 0);
        eclass_addmethod(c, (method) rslider_preset,          "preset",           A_NULL, 0);
        
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 120.");
        
        CLASS_ATTR_LONG                 (c, "listmode", 0, t_rslider, f_mode);
        CLASS_ATTR_LABEL                (c, "listmode", 0, "List Mode");
        CLASS_ATTR_FILTER_CLIP          (c, "listmode", 0, 1);
        CLASS_ATTR_ORDER                (c, "listmode", 0, "1");
        CLASS_ATTR_DEFAULT              (c, "listmode", 0, "0");
        CLASS_ATTR_SAVE                 (c, "listmode", 1);
        CLASS_ATTR_STYLE                (c, "listmode", 0, "onoff");
        
        CLASS_ATTR_FLOAT                (c, "min", 0, t_rslider, f_min);
        CLASS_ATTR_LABEL                (c, "min", 0, "Minimum Value");
        CLASS_ATTR_ORDER                (c, "min", 0, "1");
        CLASS_ATTR_DEFAULT              (c, "min", 0, "0.");
        CLASS_ATTR_SAVE                 (c, "min", 1);
        CLASS_ATTR_STYLE                (c, "min", 0, "number");
        
        CLASS_ATTR_FLOAT                (c, "max", 0, t_rslider, f_max);
        CLASS_ATTR_LABEL                (c, "max", 0, "Maximum Value");
        CLASS_ATTR_ORDER                (c, "max", 0, "1");
        CLASS_ATTR_DEFAULT              (c, "max", 0, "1.");
        CLASS_ATTR_SAVE                 (c, "max", 1);
        CLASS_ATTR_STYLE                (c, "max", 0, "number");
        
        CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_rslider, f_color_background);
        CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
        CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_rslider, f_color_border);
        CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
        CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "kncolor", 0, t_rslider, f_color_knob);
        CLASS_ATTR_LABEL                (c, "kncolor", 0, "Knob Color");
        CLASS_ATTR_ORDER                (c, "kncolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "kncolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "kncolor", 0, "color");
        
        eclass_register(CLASS_BOX, c);
        rslider_class = c;        
    }
}








