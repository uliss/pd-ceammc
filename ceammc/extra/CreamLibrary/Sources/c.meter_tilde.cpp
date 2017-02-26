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

typedef struct  _meter
{
	t_edspbox   j_box;
	t_clock*	f_clock;
	char        f_startclock;
	long        f_interval;
    t_outlet*   f_outlet;
    float       f_peak;
	char        f_direction;
    long		f_overled;
    t_rgba		color_background;
    t_rgba		color_border;
	t_rgba		f_color_signal_cold;
	t_rgba		f_color_signal_tepid;
	t_rgba		f_color_signal_warm;
	t_rgba		f_color_signal_hot;
	t_rgba		f_color_signal_over;
	
} t_meter;

static t_eclass *meter_class;

static void meter_output(t_meter *x)
{
    t_pd* send = ebox_getsender((t_ebox *) x);
    outlet_float(x->f_outlet, x->f_peak);
    if(send)
    {
        pd_float(send, x->f_peak);
    }
}

static void meter_tick(t_meter *x)
{
    if(canvas_dspstate)
    {
        if(x->f_peak > 0.)
        {
            x->f_peak = 20. * log10(x->f_peak);
        }
        else
        {
            x->f_peak = -90.;
        }
        if(x->f_peak >= 0)
        {
            x->f_overled = 1;
        }
        else if(x->f_overled > 0)
        {
            x->f_overled++;
        }
        if(x->f_overled >= 1000. / x->f_interval)
        {
            x->f_overled = 0;
        }
        
        meter_output(x);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_leds_layer);
        ebox_redraw((t_ebox *)x);
        clock_delay(x->f_clock, x->f_interval);
    }
}

static void meter_perform(t_meter *x, t_object *dsp, t_sample **ins, long ni, t_sample **outs, long no, long nsamples, long f,void *up)
{
    int i;
    t_sample peak = fabs(ins[0][0]), temp;
    for(i = 1; i < nsamples; i++)
    {
        temp = fabs(ins[0][i]);
        if(temp > peak)
        {
            peak = temp;
        }
    }
    x->f_peak = (float)peak;
    if(x->f_startclock)
    {
        x->f_startclock = 0;
        clock_delay(x->f_clock, 0);
    }
}

static void meter_dsp(t_meter *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add"), x, (method)meter_perform, 0, NULL);
    x->f_startclock = 1;
}

static void meter_getdrawparams(t_meter *x, t_object *patcherview, t_edrawparams *params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

static t_pd_err meter_notify(t_meter *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    if (msg == cream_sym_attr_modified)
    {
        if(s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_coldcolor || s == cream_sym_tepidcolor || s == cream_sym_warmcolor || s == cream_sym_hotcolor || s == cream_sym_overcolor)
        {
            ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
            ebox_invalidate_layer((t_ebox *)x, cream_sym_leds_layer);
        }
    }
    return 0;
}

static void meter_oksize(t_meter *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8.);
    newrect->height = pd_clip_min(newrect->height, 8.);
    if(newrect->width > newrect->height)
    {
        x->f_direction = 1;
    }
    else
    {
        x->f_direction = 0;
    }
    
    if(x->f_direction)
    {
        newrect->width = pd_clip_min(newrect->width, 50.);
    }
    else
    {
        newrect->height = pd_clip_min(newrect->height, 50.);
    }
}

static void draw_background(t_meter *x,  t_object *view, t_rect *rect)
{
    int i;
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect->width, rect->height);
    if (g)
    {
        egraphics_set_color_rgba(g, &x->color_border);
        if(!x->f_direction)
        {
            float ratio = rect->height / 13.f;
            for(i = 1; i < 13; i++)
            {
                egraphics_move_to(g, 0., i * ratio);
                egraphics_line_to(g, rect->width, i * ratio);
                egraphics_stroke(g);
            }
        }
        else
        {
            float ratio = rect->width / 13.f;
            for(i = 1; i < 13; i++)
            {
                egraphics_move_to(g, i * ratio, 0.f);
                egraphics_line_to(g, i * ratio, rect->height);
                egraphics_stroke(g);
            }
        }
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0.f, 0.f);
}


static void draw_leds(t_meter *x, t_object *view, t_rect *rect)
{
    float i;
    float dB;
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_leds_layer, rect->width, rect->height);
    
    if (g)
    {
        const float peak = x->f_peak;
        const long overload = x->f_overled;
        float led_height = rect->height / 13.f;
        float led_width = rect->width / 13.f;
        for(i = 12, dB = -39; i > 0; i--, dB += 3.f)
        {
            if(peak >= dB)
            {
                if(i > 9)
                    egraphics_set_color_rgba(g, &x->f_color_signal_cold);
                else if(i > 6)
                    egraphics_set_color_rgba(g, &x->f_color_signal_tepid);
                else if(i > 3)
                    egraphics_set_color_rgba(g, &x->f_color_signal_warm);
                else if(i > 0)
                    egraphics_set_color_rgba(g, &x->f_color_signal_hot);
                if(!x->f_direction)
                {
                    if(i > 11)
                        egraphics_rectangle_rounded(g, 0, i * led_height + 1, rect->width, led_height, 1.f);
                    else
                        egraphics_rectangle_rounded(g, 0, i * led_height + 1, rect->width, led_height - 1, 1.f);
                }
                else
                {
                    if(i > 11)
                        egraphics_rectangle_rounded(g, 0, 0, led_width, rect->height, 1.);
                    else
                        egraphics_rectangle_rounded(g, (12 - i) * led_width + 1, 0, led_width - 1, rect->height, 1.);
                }
                egraphics_fill(g);
            }
        }
        if(overload)
        {
            egraphics_set_color_rgba(g, &x->f_color_signal_over);
            if(!x->f_direction)
            {
                egraphics_rectangle_rounded(g, 0, 0, rect->width, led_height, 1.f);
            }
            else
            {
                egraphics_rectangle_rounded(g, 12 * led_width + 1, 0, led_width,  rect->height, 1.);
                
            }
            
            egraphics_fill(g);
        }
        ebox_end_layer((t_ebox *)x, cream_sym_leds_layer);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_leds_layer, 0., 0.);
}

static void meter_paint(t_meter *x, t_object *view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
    draw_leds(x, view, &rect);
}

static void *meter_new(t_symbol *s, int argc, t_atom *argv)
{
    t_meter *x  = (t_meter *)eobj_new(meter_class);
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    if(x && d)
    {
        ebox_new((t_ebox *)x, 0 | EBOX_GROWINDI | EBOX_IGNORELOCKCLICK);
        eobj_dspsetup((t_ebox *)x, 1, 0);
        x->f_direction      = 0;
        x->f_outlet   = outlet_new((t_object *)x, &s_float);
        x->f_peak     = -90.;
        x->f_clock          = clock_new(x,(t_method)meter_tick);
        x->f_startclock     = 0;
        x->f_overled = 0;
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
    }
    
    return (x);
}

static void meter_free(t_meter *x)
{
    ebox_free((t_ebox *)x);
    clock_free(x->f_clock);
}


extern "C" void setup_ui0x2emeter_tilde(void)
{
    t_eclass *c;
    
    c = eclass_new("ui.meter~", (method)meter_new, (method)meter_free, (short)sizeof(t_meter), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);
    eclass_guiinit(c, 0);
    
    eclass_addmethod(c, (method) meter_dsp,             "dsp",              A_NULL, 0);
    eclass_addmethod(c, (method) meter_paint,           "paint",            A_NULL, 0);
    eclass_addmethod(c, (method) meter_notify,          "notify",           A_NULL, 0);
    eclass_addmethod(c, (method) meter_getdrawparams,   "getdrawparams",    A_NULL, 0);
    eclass_addmethod(c, (method) meter_oksize,          "oksize",           A_NULL, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
    CLASS_ATTR_DEFAULT              (c, "size", 0, "13 85");
    
    CLASS_ATTR_LONG                 (c, "interval", 0, t_meter, f_interval);
    CLASS_ATTR_ORDER                (c, "interval", 0, "1");
    CLASS_ATTR_LABEL                (c, "interval", 0, "Refresh Interval in Milliseconds");
    CLASS_ATTR_FILTER_MIN           (c, "interval", 20);
    CLASS_ATTR_DEFAULT              (c, "interval", 0, "50");
    CLASS_ATTR_SAVE                 (c, "interval", 1);
    CLASS_ATTR_STYLE                (c, "interval", 0, "number");

    ATTR_DEFAULT_COLOR_BORDER       (c, t_meter);
    ATTR_DEFAULT_COLOR_BACKGROUND   (c, t_meter);
    
    CLASS_ATTR_RGBA                 (c, "coldcolor", 0, t_meter, f_color_signal_cold);
    CLASS_ATTR_LABEL                (c, "coldcolor", 0, "Cold Signal Color");
    CLASS_ATTR_ORDER                (c, "coldcolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "coldcolor", 0, "0. 0.6 0. 0.8");
    CLASS_ATTR_STYLE                (c, "coldcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "tepidcolor", 0, t_meter, f_color_signal_tepid);
    CLASS_ATTR_LABEL                (c, "tepidcolor", 0, "Tepid Signal Color");
    CLASS_ATTR_ORDER                (c, "tepidcolor", 0, "4");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "tepidcolor", 0, "0.6 0.73 0. 0.8");
    CLASS_ATTR_STYLE                (c, "tepidcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "warmcolor", 0, t_meter, f_color_signal_warm);
    CLASS_ATTR_LABEL                (c, "warmcolor", 0, "Warm Signal Color");
    CLASS_ATTR_ORDER                (c, "warmcolor", 0, "5");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "warmcolor", 0, ".85 .85 0. 0.8");
    CLASS_ATTR_STYLE                (c, "warmcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "hotcolor", 0, t_meter, f_color_signal_hot);
    CLASS_ATTR_LABEL                (c, "hotcolor", 0, "Hot Signal Color");
    CLASS_ATTR_ORDER                (c, "hotcolor", 0, "6");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "hotcolor", 0, "1. 0.6 0. 0.8");
    CLASS_ATTR_STYLE                (c, "hotcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "overcolor", 0, t_meter, f_color_signal_over);
    CLASS_ATTR_LABEL                (c, "overcolor", 0, "Overload Signal Color");
    CLASS_ATTR_ORDER                (c, "overcolor", 0, "7");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "overcolor", 0, "1. 0. 0. 0.8");
    CLASS_ATTR_STYLE                (c, "overcolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
    meter_class = c;
}



