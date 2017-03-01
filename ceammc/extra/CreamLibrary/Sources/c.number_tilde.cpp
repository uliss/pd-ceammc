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

typedef struct  _number_tilde
{
	t_edspbox   j_box;
	t_clock*	f_clock;
	int			f_startclock;
	long        f_interval;
    long        f_ndecimal;
    t_outlet*   f_peaks_outlet;
    float       f_peak_value;
    int         f_max_decimal;
    t_rgba		color_background;
    t_rgba		color_border;
    t_rgba		color_text;

} t_number_tilde;

static t_eclass *number_tilde_class;

static void number_tilde_output(t_number_tilde *x)
{
    t_pd* sender = ebox_getsender((t_ebox *) x);
    outlet_float(x->f_peaks_outlet, x->f_peak_value);
    if(sender)
    {
        pd_float(sender, x->f_peak_value);
    }
}

static void number_tilde_perform(t_number_tilde *x, t_object *dsp, t_sample **ins, long ni, t_sample **outs, long no, long nsamples, long f,void *up)
{
    x->f_peak_value = ins[0][0];
    memcpy(outs[0], ins[0], (size_t)nsamples * sizeof(t_sample));
    if(x->f_startclock)
    {
        x->f_startclock = 0;
        clock_delay(x->f_clock, 0);
    }
}

static void number_tilde_dsp(t_number_tilde *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(count[0])
    {
        object_method(dsp, gensym("dsp_add"), x, (method)number_tilde_perform, 0, NULL);
        x->f_startclock = 1;
    }
}

static void number_tilde_tick(t_number_tilde *x)
{
	if(canvas_dspstate)
    {
        number_tilde_output(x);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_value_layer);
        ebox_redraw((t_ebox *)x);
		clock_delay(x->f_clock, x->f_interval);
    }
}

static t_pd_err number_tilde_notify(t_number_tilde *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == cream_sym_attr_modified)
	{
		if(s == cream_sym_fontsize || s == cream_sym_fontname || s == cream_sym_fontweight || s == cream_sym_fontslant)
		{
			ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
			ebox_invalidate_layer((t_ebox *)x, cream_sym_value_layer);
		}
        if(s == cream_sym_fontsize)
        {
            eobj_attr_setvalueof(x, s_size, 0, NULL);
        }
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

static void draw_background(t_number_tilde *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect->width, rect->height);
    t_etext *jtl;
	if(g)
	{
        jtl = etext_layout_create();
        if(jtl)
        {
            etext_layout_set(jtl, "~", &x->j_box.b_font, 1, rect->height / 2., rect->width, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_settextcolor(jtl, &x->color_text);
            etext_layout_draw(jtl, g);
            
            egraphics_set_line_width(g, 1);     //Cream: 2
            egraphics_set_color_rgba(g, &x->color_border);
            egraphics_move_to(g, 0, 0);
            egraphics_line_to(g, sys_fontwidth(x->j_box.b_font.c_size) + 6,  rect->height / 2.);
            egraphics_line_to(g, 0, rect->height);
            egraphics_stroke(g);
            
            ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
            etext_layout_destroy(jtl);
        }
	}
	ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0., 0.);
}

static void draw_value(t_number_tilde *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_value_layer, rect->width, rect->height);
	if(g)
	{
        t_etext *jtl = etext_layout_create();
        if(jtl)
        {
            char number[256];
            if(x->f_max_decimal == 0)
                sprintf(number, "%i", (int)x->f_peak_value);
            else if(x->f_max_decimal == 1)
                sprintf(number, "%.1f", x->f_peak_value);
            else if(x->f_max_decimal == 2)
                sprintf(number, "%.2f", x->f_peak_value);
            else if(x->f_max_decimal == 3)
                sprintf(number, "%.3f", x->f_peak_value);
            else if(x->f_max_decimal == 4)
                sprintf(number, "%.4f", x->f_peak_value);
            else if(x->f_max_decimal == 5)
                sprintf(number, "%.5f", x->f_peak_value);
            else
                sprintf(number, "%.6f", x->f_peak_value);
            etext_layout_settextcolor(jtl, &x->color_text);
            etext_layout_set(jtl, number, &x->j_box.b_font, sys_fontwidth(x->j_box.b_font.c_size) + 8, rect->height / 2., rect->width - 3, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            
            etext_layout_draw(jtl, g);
            ebox_end_layer((t_ebox*)x, cream_sym_value_layer);
            etext_layout_destroy(jtl);
        }
	}
	ebox_paint_layer((t_ebox *)x, cream_sym_value_layer, 0., 0.);
}

static void number_tilde_paint(t_number_tilde *x, t_object *view)
{
    t_rect rect;
#ifdef __APPLE__
    float fontwidth = sys_fontwidth(x->j_box.b_font.c_size);
#elif _WINDOWS
    float fontwidth = sys_fontwidth(x->j_box.b_font.c_size);
#else
    float fontwidth = sys_fontwidth(x->j_box.b_font.c_size) + 3;
#endif
    
    ebox_get_rect_for_view((t_ebox *)x, &rect);
#ifdef __APPLE__
    x->f_max_decimal = (rect.width - fontwidth - 8) / fontwidth - 2;
#elif _WINDOWS
    x->f_max_decimal = (rect.width - fontwidth - 8) / fontwidth - 2;
#else
    x->f_max_decimal = (rect.width - fontwidth - 11) / fontwidth + 1;
#endif
    draw_background(x, view, &rect);
    draw_value(x, view, &rect);
}

static void number_tilde_getdrawparams(t_number_tilde *x, t_object *patcherview, t_edrawparams *params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

static void number_tilde_oksize(t_number_tilde *x, t_rect *newrect)
{
#ifdef __APPLE__
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.b_font.c_size) * 3 + 8);
#elif _WINDOWS
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.b_font.c_size) * 3 + 8);
#else
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.b_font.c_size) * 3 + 11);
#endif
    newrect->height = sys_fontheight(x->j_box.b_font.c_size) + 4;
}

static void number_tilde_free(t_number_tilde *x)
{
    clock_free(x->f_clock);
    ebox_free((t_ebox *)x);
}

static void *number_tilde_new(t_symbol *s, int argc, t_atom *argv)
{
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    t_number_tilde *x = (t_number_tilde *)eobj_new(number_tilde_class);
    if(x && d)
    {
        ebox_new((t_ebox *)x, 0 | EBOX_GROWINDI | EBOX_IGNORELOCKCLICK);
        eobj_dspsetup((t_ebox *)x, 1, 1);
        x->f_peaks_outlet   = outlet_new((t_object *)x, &s_float);
        x->f_peak_value     = 0.;
        x->f_clock          = clock_new(x,(t_method)number_tilde_tick);
        x->f_startclock     = 0;
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
    }
    
    return (x);
}

extern "C" void setup_ui0x2enumber_tilde(void)
{
    t_eclass *c;
    
    c = eclass_new("ui.number~", (method)number_tilde_new, (method)number_tilde_free, (short)sizeof(t_number_tilde), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);
    eclass_guiinit(c, 0);

    
    eclass_addmethod(c, (method) number_tilde_dsp,             "dsp",              A_NULL, 0);
    eclass_addmethod(c, (method) number_tilde_paint,           "paint",            A_NULL, 0);
    eclass_addmethod(c, (method) number_tilde_notify,          "notify",           A_NULL, 0);
    eclass_addmethod(c, (method) number_tilde_getdrawparams,   "getdrawparams",    A_NULL, 0);
    eclass_addmethod(c, (method) number_tilde_oksize,          "oksize",           A_NULL, 0);
    eclass_addmethod(c, (method) number_tilde_output,          "bang",             A_NULL, 0);
    
    CLASS_ATTR_DEFAULT              (c, "size", 0, "53 16");
    
    CLASS_ATTR_LONG                 (c, "interval", 0, t_number_tilde, f_interval);
    CLASS_ATTR_ORDER                (c, "interval", 0, "2");
    CLASS_ATTR_LABEL                (c, "interval", 0, "Refresh Interval in Milliseconds");
    CLASS_ATTR_FILTER_MIN           (c, "interval", 20);
    CLASS_ATTR_DEFAULT              (c, "interval", 0, "50");
    CLASS_ATTR_SAVE                 (c, "interval", 1);
    CLASS_ATTR_STYLE                (c, "interval", 0, "number");
    
    CLASS_ATTR_LONG                 (c, "decimal", 0, t_number_tilde, f_ndecimal);
    CLASS_ATTR_ORDER                (c, "decimal", 0, "3");
    CLASS_ATTR_LABEL                (c, "decimal", 0, "Number of decimal");
    CLASS_ATTR_DEFAULT              (c, "decimal", 0, "6");
    CLASS_ATTR_FILTER_MIN           (c, "decimal", 0);
    CLASS_ATTR_FILTER_MAX           (c, "decimal", 6);
    CLASS_ATTR_SAVE                 (c, "decimal", 1);
    CLASS_ATTR_STYLE                (c, "decimal", 0, "number");
    
    ATTR_DEFAULT_COLOR_BORDER       (c, t_number_tilde);
    ATTR_DEFAULT_COLOR_BACKGROUND   (c, t_number_tilde);
    ATTR_DEFAULT_COLOR_TEXT         (c, t_number_tilde);
    
    eclass_register(CLASS_BOX, c);
    number_tilde_class = c;
}







