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

typedef struct _breakpoints
{
	t_ebox      j_box;

    t_outlet*   f_out_float;
    t_outlet*   f_out_list;
    t_outlet*   f_out_function;
    t_clock*    f_clock;

    t_pt        f_size;
    t_pt        f_mouse;
    t_symbol*   f_outline;
    t_pt*       f_points;
    int         f_npoints;
    long        f_point_hover;
    long        f_point_selected;
    long        f_point_last_created;
    long        f_output_inc;
    char        f_output_nextprev;

    float       f_range_abscissa[2];
    float       f_range_ordinate[2];

	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_point;
    t_rgba		f_color_line;
    t_rgba		f_color_text;

} t_breakpoints;

t_eclass *breakpoints_class;

static float breakpoints_linear(t_breakpoints *x, float f)
{
    for(int i = 0; i < x->f_npoints - 1; i++)
    {
        if(f >= x->f_points[i].x && f <= x->f_points[i+1].x)
        {
            float ratio = (f - x->f_points[i].x) / (x->f_points[i+1].x - x->f_points[i].x);
            return x->f_points[i].y * (1. - ratio) + x->f_points[i+1].y * (ratio);
        }
    }
    return 0;
}

static float breakpoints_cosine(t_breakpoints *x, float f)
{
    for(int i = 0; i < x->f_npoints - 1; i++)
    {
        if(f >= x->f_points[i].x && f <= x->f_points[i+1].x)
        {
            float ratio1 = (f - x->f_points[i].x) / (x->f_points[i+1].x - x->f_points[i].x);
            float ratio2 = (1.f - cosf(ratio1 * EPD_PI)) * 0.5f;
            return x->f_points[i].y * (1.f - ratio2) + x->f_points[i+1].y * ratio2;
        }
    }
    return 0;
}

static float breakpoints_cubic(t_breakpoints *x, float f)
{
    float ratio1, ratio2;
    float y0, y1, y2, y3;
    float a0, a1, a2, a3;
    for(int i = 0; i < x->f_npoints - 1; i++)
    {
        if(f >= x->f_points[i].x && f <= x->f_points[i+1].x)
        {
            ratio1 = (f - x->f_points[i].x) / (x->f_points[i+1].x - x->f_points[i].x);
            ratio2 = ratio1*ratio1;
            if(i == 0)
            {
                y1 = y0 = x->f_points[i].y;
                y2 = x->f_points[i+1].y;
                y3 = x->f_points[i+2].y;
            }
            else if(i == x->f_npoints - 2)
            {
                y0 = x->f_points[i-1].y;
                y1 = x->f_points[i].y;
                y3 = y2 = x->f_points[i+1].y;
            }
            else
            {
                y0 = x->f_points[i-1].y;
                y1 = x->f_points[i].y;
                y2 = x->f_points[i+1].y;
                y3 = x->f_points[i+2].y;
            }
            
            a0 = y3 - y2 - y0 + y1;
            a1 = y0 - y1 - a0;
            a2 = y2 - y0;
            a3 = y1;
            
            return a0 * ratio1 * ratio2 + a1 * ratio2 + a2 * ratio1 + a3;
        }
    }
    return 0;
}

static float breakpoints_interpolation(t_breakpoints *x, float f)
{
    f = pd_clip_minmax(f, x->f_range_abscissa[0], x->f_range_abscissa[1]);
    if(x->f_outline == cream_sym_Linear)
        return breakpoints_linear(x, f);
    else if(x->f_outline == cream_sym_Cosine || x->f_npoints == 2)
        return breakpoints_cosine(x, f);
    else
        return breakpoints_cubic(x, f);
}

static void breakpoints_float(t_breakpoints *x, float f)
{
    float result;
    if(x->f_npoints == 0)
        return;
    if(x->f_npoints == 1)
    {
        outlet_float(x->f_out_float, x->f_points[0].y);
        return;
    }
    result = breakpoints_interpolation(x, f);
    outlet_float(x->f_out_float, result);
}


static void breakpoints_getlist(t_breakpoints *x)
{
    if(x->f_npoints)
    {
        t_atom* av = (t_atom *)malloc((size_t)(x->f_npoints * 2) * sizeof(t_atom));
        if(av)
        {
            for(int i = 0, j = 0; i < x->f_npoints; j += 2, i++)
            {
                atom_setfloat(av+j, x->f_points[i].y);
                atom_setfloat(av+j+1, x->f_points[i].x);
            }
            outlet_list(x->f_out_function, &s_list, x->f_npoints * 2, av);
            free(av);
        }
        else
        {
            pd_error(x, "can't allocate memory.");
        }
    }
    
}

static void breakpoints_bang(t_breakpoints *x)
{
    x->f_output_inc = 0;
    if(x->f_npoints)
    {
        t_atom av[2];
        atom_setfloat(av, x->f_points[0].y);
        atom_setfloat(av+1, x->f_points[0].x);
        outlet_list(x->f_out_list, &s_list, 2, av);
        if(x->f_npoints > 1)
        {
            clock_delay(x->f_clock, x->f_points[1].x - x->f_points[0].x);
        }
    }
}

static void breakpoints_inc(t_breakpoints *x)
{
    x->f_output_inc++;
    if(x->f_output_inc < x->f_npoints)
    {
        t_atom av[2];
        atom_setfloat(av, x->f_points[x->f_output_inc].y);
        atom_setfloat(av+1, x->f_points[x->f_output_inc].x);
        outlet_list(x->f_out_list, &s_list, 2, av);
        if(x->f_npoints > x->f_output_inc )
        {
            clock_delay(x->f_clock, x->f_points[x->f_output_inc+1].x - x->f_points[x->f_output_inc].x);
            return;
        }
    }
}

static void breakpoints_next(t_breakpoints *x)
{
    if(x->f_npoints)
    {
        t_atom av[2];
        x->f_output_nextprev++;
        if(x->f_output_nextprev >= x->f_npoints)
            x->f_output_nextprev = 0;
        atom_setfloat(av, (float)x->f_points[(int)x->f_output_nextprev].y);
        atom_setfloat(av+1, (float)x->f_points[(int)x->f_output_nextprev].x);
        outlet_list(x->f_out_list, &s_list, 2, av);
    }
}

static void breakpoints_prev(t_breakpoints *x)
{
    if(x->f_npoints)
    {
        t_atom av[2];
        x->f_output_nextprev--;
        if(x->f_output_nextprev < 0)
            x->f_output_nextprev = x->f_npoints - 1;
        atom_setfloat(av, (float)x->f_points[(int)x->f_output_nextprev].y);
        atom_setfloat(av+1, (float)x->f_points[(int)x->f_output_nextprev].x);
        outlet_list(x->f_out_list, &s_list, 2, av);
    }
}

static void breakpoints_erase(t_breakpoints *x)
{
    x->f_npoints = 0;
    free(x->f_points);
    x->f_points = NULL;
    ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
    ebox_redraw((t_ebox *)x);
}

static void breakpoints_add(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    if(argc && argv)
    {
        if(argc == 2 && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT)
        {
            float abs = pd_clip_minmax(atom_getfloat(argv), x->f_range_abscissa[0], x->f_range_abscissa[1]);
            float ord = pd_clip_minmax(atom_getfloat(argv+1), x->f_range_ordinate[0], x->f_range_ordinate[1]);
            if(!x->f_points)
            {
                x->f_points = (t_pt *)malloc(sizeof(t_pt));
                if(x->f_points)
                {
                    x->f_npoints = 1;
                    x->f_points[0].x = abs;
                    x->f_points[0].y = ord;
                    x->f_point_last_created = 0;
                }
                else
                {
                    x->f_point_last_created = -1;
                    pd_error(x, "can't allocate memory.");
                }
            }
            else if(abs >= x->f_points[x->f_npoints-1].x)
            {
                x->f_points = (t_pt *)realloc(x->f_points, (size_t)(x->f_npoints + 1) * sizeof(t_pt));
                if(x->f_points)
                {
                    x->f_points[x->f_npoints].x = abs;
                    x->f_points[x->f_npoints].y = ord;
                    x->f_npoints++;
                    x->f_point_last_created = x->f_npoints-1;
                }
                else
                {
                    x->f_npoints = 0;
                     x->f_point_last_created = -1;
                    pd_error(x, "can't allocate memory.");
                }
            }
            else if(abs <= x->f_points[0].x)
            {
                x->f_points = (t_pt *)realloc(x->f_points, (size_t)(x->f_npoints + 1) * sizeof(t_pt));
                if(x->f_points)
                {
                    memcpy(x->f_points+1, x->f_points,(size_t) (x->f_npoints) * sizeof(t_pt));
                    x->f_points[0].x = abs;
                    x->f_points[0].y = ord;
                    x->f_npoints++;
                    x->f_point_last_created = 0;
                }
                else
                {
                    x->f_npoints = 0;
                     x->f_point_last_created = -1;
                    pd_error(x, "can't allocate memory.");
                }
                
            }
            else
            {
                int index = 0;
                for(int i = 1; i < x->f_npoints; i++)
                {
                    if(x->f_points[i].x > abs)
                    {
                        index = i;
                        break;
                    }
                }
                x->f_points = (t_pt *)realloc(x->f_points, (size_t)(x->f_npoints + 1) * sizeof(t_pt));
                if(x->f_points)
                {
                    memcpy(x->f_points+index+1, x->f_points+index, (size_t)(x->f_npoints - index) * sizeof(t_pt));
                    x->f_points[index].x = abs;
                    x->f_points[index].y = ord;
                    x->f_npoints++;
                    x->f_point_last_created = index;
                }
                else
                {
                    x->f_npoints = 0;
                    x->f_point_last_created = -1;
                    pd_error(x, "can't allocate memory.");
                }
            }
            ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
            ebox_redraw((t_ebox *)x);
        }
    }
}

static void breakpoints_remove(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    if(argc && argv)
    {
        if(argc == 1 && atom_gettype(argv) == A_FLOAT)
        {
            int index = pd_clip_minmax(atom_getfloat(argv), 0, x->f_npoints-1);
            if(x->f_npoints > 1)
            {
                memcpy(x->f_points+index, x->f_points+index+1, (size_t)(x->f_npoints - index - 1) * sizeof(t_pt));
                x->f_points = (t_pt *)realloc(x->f_points, (size_t)(x->f_npoints - 1) * sizeof(t_pt));
            }
            else
            {
                free(x->f_points);
                x->f_points = NULL;
            }
            if(x->f_points)
            {
                x->f_npoints--;
            }
            else
            {
                x->f_npoints = 0;
            }
            
            ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
            ebox_redraw((t_ebox *)x);
        }
    }
}

static void breakpoints_move(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    if(argv && argc > 2 && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT)
    {
        int index = atom_getfloat(argv);
        if(index >= 0 && index < x->f_npoints)
        {
            float abs = atom_getfloat(argv+1);
            float ord = pd_clip_minmax(atom_getfloat(argv+2), x->f_range_ordinate[0], x->f_range_ordinate[1]);
            if(index == 0)
            {
                if(x->f_npoints > 1)
                    x->f_points[0].x = pd_clip_minmax(abs, x->f_range_abscissa[0], x->f_points[1].x);
                else
                    x->f_points[0].x = pd_clip_minmax(abs, x->f_range_abscissa[0], x->f_range_abscissa[1]);
                x->f_points[0].y = ord;
            }
            else if(index == x->f_npoints-1)
            {
                x->f_points[index].x = pd_clip_minmax(abs, x->f_points[index-1].x, x->f_range_abscissa[1]);
                x->f_points[index].y = ord;
            }
            else
            {
                x->f_points[index].x = pd_clip_minmax(abs, x->f_points[index-1].x, x->f_points[index+1].x);
                x->f_points[index].y = ord;
            }
            
            ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
            ebox_redraw((t_ebox *)x);
        }
    }
}

static void breakpoints_scaleabs(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    int i;
    float min, max, ratio;
    if(argc >= 2 && argv && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT)
    {
        min = atom_getfloat(argv);
        max = atom_getfloat(argv+1);
        if(min > max)
        {
            min = max;
            max = atom_getfloat(argv);
        }
        
        ratio = x->f_range_abscissa[1] - x->f_range_abscissa[0];
        for(i = 0; i < x->f_npoints; i++)
        {
            x->f_points[i].x = ((x->f_points[i].x - x->f_range_abscissa[0]) / ratio) * (max - min) + min;
        }
        eobj_attr_setvalueof(x, cream_sym_absrange, argc, argv);
    }
    
}

static void breakpoints_scaleord(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    int i;
    float min, max, ratio;
    if(argc >= 2 && argv && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT)
    {
        min = atom_getfloat(argv);
        max = atom_getfloat(argv+1);
        if(min > max)
        {
            min = max;
            max = atom_getfloat(argv);
        }
        
        ratio = x->f_range_ordinate[1] - x->f_range_ordinate[0];
        for(i = 0; i < x->f_npoints; i++)
        {
            x->f_points[i].y = ((x->f_points[i].y - x->f_range_ordinate[0]) / ratio) * (max - min) + min;
        }
        eobj_attr_setvalueof(x, cream_sym_ordrange, argc, argv);
    }
}

static void breakpoints_init(t_breakpoints *x, t_binbuf *d)
{
    long ac = binbuf_getnatom(d);
    t_atom* av = binbuf_getvec(d);
    breakpoints_erase(x);
    for(int i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYMBOL && atom_getsymbol(av+i) == cream_sym_atpoints)
        {
            for(i = i+1;i < ac-1; i += 2)
            {
                if(atom_gettype(av+i) == A_FLOAT && atom_gettype(av+i+1) == A_FLOAT)
                {
                    breakpoints_add(x, NULL, 2, av+i);
                }
                else
                {
                    pd_error(x, "invalid point syntax.");
                }
            }
        }
    }
}


static void breakpoints_preset(t_breakpoints *x, t_binbuf *b)
{
    binbuf_addv(b, (char *)"s", cream_sym_function);
    for(int i = 0; i < x->f_npoints; i++)
    {
        binbuf_addv(b, (char *)"ff", (float)x->f_points[i].x, (float)x->f_points[i].y);
    }
}

static void breakpoints_save(t_breakpoints *x, t_binbuf *d)
{
    if(x->f_npoints)
    {
        binbuf_addv(d, (char *)"s", cream_sym_atpoints);
        for(int i = 0; i < x->f_npoints; i++)
        {
            binbuf_addv(d, (char *)"ff", x->f_points[i].x, x->f_points[i].y);
        }
    }
}

static void breakpoints_read(t_breakpoints *x, t_symbol *s, int argc, t_atom *argv)
{
    t_binbuf *d = binbuf_new();
    if(d && argv && argc && atom_gettype(argv) == A_SYMBOL)
    {
        if(binbuf_read(d, atom_getsymbol(argv)->s_name, (char *)"", 0))
        {
            pd_error(x, "breakpoints : %s read failed", atom_getsymbol(argv)->s_name);
        }
        else
        {
            breakpoints_init(x, d);
        }
    }
    if(d)
        binbuf_free(d);
}

static void breakpoints_write(t_breakpoints *x, t_symbol *s, int argc, t_atom *argv)
{
    t_binbuf *d = binbuf_new();
    if(d && argv && argc && atom_gettype(argv) == A_SYMBOL)
    {
        breakpoints_save(x, d);
        if(binbuf_write(d, atom_getsymbol(argv)->s_name, (char *)"", 0))
        {
            pd_error(x, "breakpoints : %s write failed", atom_getsymbol(argv)->s_name);
        }
    }
    if(d)
        binbuf_free(d);
    
}

static void breakpoints_function(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    t_binbuf* b = binbuf_new();
    if(argc && argv)
    {
        binbuf_addv(b, (char *)"s", cream_sym_atpoints);
        binbuf_add(b, argc, argv);
        breakpoints_init(x, b);
        binbuf_free(b);
    }
}

static void breakpoints_getdrawparams(t_breakpoints *x, t_object *patcherview, t_edrawparams *params)
{
    params->d_borderthickness   = 2;
    params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

static void breakpoints_oksize(t_breakpoints *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

static t_pd_err breakpoints_notify(t_breakpoints *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    if (msg == cream_sym_attr_modified)
    {
        if(s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_ptcolor || s == cream_sym_licolor || s == cream_sym_textcolor || s == cream_sym_fontsize || s == cream_sym_fontname || s ==cream_sym_fontweight || s == cream_sym_fontslant)
        {
            ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
            ebox_invalidate_layer((t_ebox *)x, cream_sym_text_layer);
        }
        else if(s == cream_sym_absrange || s == cream_sym_ordrange)
        {
            if(x->f_range_abscissa[0] > x->f_range_abscissa[1])
            {
                float max = x->f_range_abscissa[0];
                x->f_range_abscissa[0] = x->f_range_abscissa[1];
                x->f_range_abscissa[1] = max;
            }
            if(x->f_range_ordinate[0] > x->f_range_ordinate[1])
            {
                float max = x->f_range_ordinate[0];
                x->f_range_ordinate[0] = x->f_range_ordinate[1];
                x->f_range_ordinate[1] = max;
            }
            for(int i = 0; i < x->f_npoints; i++)
            {
                x->f_points[i].x = pd_clip_minmax(x->f_points[i].x, x->f_range_abscissa[0], x->f_range_abscissa[1]);
                x->f_points[i].y = pd_clip_minmax(x->f_points[i].y, x->f_range_ordinate[0], x->f_range_ordinate[1]);
            }
            ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
        }
        else if(s == cream_sym_outline)
        {
            if(x->f_outline != cream_sym_Cosine && x->f_outline == cream_sym_Cubic)
            {
                x->f_outline = cream_sym_Linear;
            }
            ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
        }
        ebox_redraw((t_ebox *)x);
    }
    return 0;
}

static void breakpoints_mousemove(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int i;
    float abs, ord;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    float distx = (3. / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]);
    float disty = (3. / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]);
    
    abs = ((pt.x - 3.) / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]) + x->f_range_abscissa[0];
    ord = ((x->f_size.y - (pt.y - 4.) - 4.) / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]) + x->f_range_ordinate[0];
    x->f_point_hover = -1;
    x->f_mouse.x = abs;
    x->f_mouse.y = ord;
    
    for(i = 0; i < x->f_npoints; i++)
    {
        if(abs > x->f_points[i].x - distx && abs < x->f_points[i].x + distx && ord > x->f_points[i].y - disty && ord < x->f_points[i].y + disty)
        {
            x->f_point_hover = i;
        }
    }
    ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_text_layer);
    ebox_redraw((t_ebox *)x);
}

static void breakpoints_mousedown(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int i;
    t_atom av[2];
    float abs, ord;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    float distx = (3. / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]);
    float disty = (3. / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]);
    
    abs = ((pt.x - 3.) / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]) + x->f_range_abscissa[0];
    ord = ((x->f_size.y - (pt.y - 4.) - 4.) / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]) + x->f_range_ordinate[0];
    
    x->f_point_selected = -1;
    x->f_point_hover    = -1;
    x->f_point_last_created = -1;
    x->f_mouse.x = abs;
    x->f_mouse.y = ord;
    
    ebox_invalidate_layer((t_ebox *)x, cream_sym_text_layer);
    if(modifiers == EMOD_SHIFT)
    {
        atom_setfloat(av, abs);
        atom_setfloat(av+1, ord);
        breakpoints_add(x, NULL, 2, av);
        x->f_point_selected = x->f_point_last_created;
        return;
    }
    
    for(i = 0; i < x->f_npoints; i++)
    {
        if(abs > x->f_points[i].x - distx && abs < x->f_points[i].x + distx && ord > x->f_points[i].y - disty && ord < x->f_points[i].y + disty)
        {
            x->f_point_selected = i;
            x->f_point_hover    = i;
            
        }
    }
    if(modifiers == EMOD_CTRL)
    {
        atom_setfloat(av, x->f_point_selected);
        breakpoints_remove(x, NULL, 1, av);
        x->f_point_selected = -1;
        x->f_point_hover    = -1;
        return;
    }
    else if(modifiers == EMOD_CTRL)
    {
        // Sustain ?
    }
}

static void breakpoints_mousedrag(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_atom av[3];
    float abs, ord;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    abs = ((pt.x - 3.) / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]) + x->f_range_abscissa[0];
    ord = ((x->f_size.y - (pt.y - 4.) - 4.) / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]) + x->f_range_ordinate[0];
    
    x->f_mouse.x = abs;
    x->f_mouse.y = ord;
    ebox_invalidate_layer((t_ebox *)x, cream_sym_text_layer);
    if(x->f_point_selected != -1)
    {
        atom_setfloat(av, x->f_point_selected);
        atom_setfloat(av+1, abs);
        atom_setfloat(av+2, ord);
        breakpoints_move(x, NULL, 3, av);
        return;
    }
}

static void breakpoints_mouseleave(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_point_selected = -1;
    x->f_point_hover    = -1;
    x->f_mouse.x = -666666;
    x->f_mouse.y = -666666;
    
    ebox_invalidate_layer((t_ebox *)x, cream_sym_text_layer);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
    ebox_redraw((t_ebox *)x);
}

static void breakpoints_mouseup(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    float abs, ord;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    abs = ((pt.x - 3.) / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]) + x->f_range_abscissa[0];
    ord = ((x->f_size.y - (pt.y - 4.) - 4.) / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]) + x->f_range_ordinate[0];
    
    
    x->f_mouse.x = abs;
    x->f_mouse.y = ord;
    x->f_point_selected    = -1;
    
    ebox_invalidate_layer((t_ebox *)x, cream_sym_text_layer);
    ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
    ebox_redraw((t_ebox *)x);
}

static void draw_text(t_breakpoints *x, t_object *view, t_rect *rect)
{
    char number[512];
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_text_layer, rect->width, rect->height);
    t_etext *jtl = etext_layout_create();
    if(g && jtl)
    {
        float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 1;
        if (x->f_point_selected != -1)
        {
            sprintf(number, "x : %.2f y : %.2f", x->f_points[x->f_point_selected].x, x->f_points[x->f_point_selected].y);
        }
        else if(x->f_mouse.x != -666666 && x->f_mouse.y != -666666)
        {
            x->f_mouse.x = pd_clip_minmax(x->f_mouse.x, x->f_range_abscissa[0], x->f_range_abscissa[1]);
            x->f_mouse.y = pd_clip_minmax(x->f_mouse.y, x->f_range_ordinate[0], x->f_range_ordinate[1]);
            sprintf(number, "x : %.2f y : %.2f", x->f_mouse.x, x->f_mouse.y);
        }
        else
        {
            ebox_end_layer((t_ebox*)x, cream_sym_text_layer);
            ebox_paint_layer((t_ebox *)x, cream_sym_text_layer, 0., 0.);
            return;
        }
        etext_layout_set(jtl, number, &x->j_box.b_font, 5, height * 0.5, rect->width, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        etext_layout_settextcolor(jtl, &x->f_color_text);
        etext_layout_draw(jtl, g);
        
        ebox_end_layer((t_ebox*)x, cream_sym_text_layer);
    }
    if(jtl)
    {
        etext_layout_destroy(jtl);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_text_layer, 0., 0.);
}

static void draw_points(t_breakpoints *x, t_object *view, t_rect *rect)
{
    int i;
    float abs, ord;
    float max, inc;
    float abs2;
    float ratiox, ratioy;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_points_layer, rect->width, rect->height);
    
    if (g && x->f_npoints)
    {
        ratiox = (rect->width - 4.) / (x->f_range_abscissa[1] - x->f_range_abscissa[0]);
        ratioy = (rect->height - height - 4.) / (x->f_range_ordinate[1] - x->f_range_ordinate[0]);
        
        egraphics_set_line_width(g, 2);
        egraphics_set_color_rgba(g, &x->f_color_line);
        
        if(x->f_outline == cream_sym_Linear)
        {
            abs = (x->f_points[0].x - x->f_range_abscissa[0]) * ratiox + 2.;
            ord = rect->height - (x->f_points[0].y - x->f_range_ordinate[0]) * ratioy - 2.;
            egraphics_move_to(g, abs, ord);
            for(i = 0; i < x->f_npoints; i++)
            {
                abs = (x->f_points[i].x - x->f_range_abscissa[0]) * ratiox + 2.;
                ord = rect->height - (x->f_points[i].y - x->f_range_ordinate[0]) * ratioy - 2.;
                egraphics_line_to(g, abs, ord);
            }
            egraphics_stroke(g);
        }
        else if (x->f_outline == cream_sym_Cosine || x->f_outline == cream_sym_Cubic)
        {
            abs = (x->f_points[0].x - x->f_range_abscissa[0]) * ratiox + 2.;
            ord = rect->height - (x->f_points[0].y - x->f_range_ordinate[0]) * ratioy - 2.;
            egraphics_move_to(g, abs, ord);
            
            max = (x->f_points[x->f_npoints-1].x - x->f_range_ordinate[0]) * ratiox + 2.;
            inc = (x->f_points[x->f_npoints-1].x - x->f_points[0].x) / (float)(max - abs);
            abs2 = x->f_points[0].x+inc;
            if(x->f_points[x->f_npoints-1].x == x->f_points[x->f_npoints-2].x)
            {
                for(i = abs; i <= max && abs2 <= x->f_points[x->f_npoints-1].x + inc; i++, abs2 += inc)
                {
                    ord = rect->height - (breakpoints_interpolation(x, abs2) - x->f_range_ordinate[0]) * ratioy - 2.;
                    egraphics_line_to(g, i, ord);
                }
            }
            else
            {
                for(i = abs; i <= max && abs2 <= x->f_points[x->f_npoints-1].x; i++, abs2 += inc)
                {
                    ord = rect->height - (breakpoints_interpolation(x, abs2) - x->f_range_ordinate[0]) * ratioy - 2.;
                    egraphics_line_to(g, i, ord);
                }
            }
            
            egraphics_stroke(g);
        }
        
        egraphics_set_color_rgba(g, &x->f_color_point);
        for(i = 0; i < x->f_npoints; i++)
        {
            abs = (x->f_points[i].x - x->f_range_abscissa[0]) * ratiox + 2;
            ord = rect->height - (x->f_points[i].y - x->f_range_ordinate[0]) * ratioy - 2;
            if(i == x->f_point_hover || i == x->f_point_selected)
            {
                egraphics_circle(g, abs, ord, 4.);
            }
            else
                egraphics_circle(g, abs, ord, 3.);
            egraphics_fill(g);
        }
    }
    if(g)
    {
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2.);
        egraphics_line_fast(g, -2, height - 1, rect->width+4, height - 1);
        ebox_end_layer((t_ebox*)x, cream_sym_points_layer);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_points_layer, 0., 0.);
}

static void breakpoints_paint(t_breakpoints *x, t_object *view)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_size.x = rect.width;
    x->f_size.y = rect.height;
    draw_text(x, view, &rect);
    draw_points(x, view, &rect);
}

static void breakpoints_free(t_breakpoints *x)
{
    breakpoints_erase(x);
    ebox_free((t_ebox *)x);
    clock_free(x->f_clock);
}

static void *breakpoints_new(t_symbol *s, int argc, t_atom *argv)
{
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    t_breakpoints *x = (t_breakpoints *)eobj_new(breakpoints_class);
    if(x && d)
    {
        x->f_outline = 0;
        long flags = 0
        | EBOX_GROWINDI
        ;
        ebox_new((t_ebox *)x, flags);
        
        x->f_out_float = outlet_new((t_object *)x, &s_float);
        x->f_out_list = outlet_new((t_object *)x, &s_list);
        x->f_out_function = outlet_new((t_object *)x, &s_list);
        x->f_npoints = 0;
        x->f_points = NULL;
        x->f_point_hover    = -1;
        x->f_point_selected = -1;
        x->f_output_inc     = -1;
        x->f_output_nextprev = 0;
        x->f_point_last_created = -1;
        x->f_mouse.x = -666666;
        x->f_mouse.y = -666666;
        
        x->f_clock = clock_new(x, (t_method)breakpoints_inc);
        
        ebox_attrprocess_viabinbuf(x, d);
        breakpoints_init(x, d);
        ebox_ready((t_ebox *)x);
    }
    
    return (x);
}

extern "C" void setup_c0x2ebreakpoints(void)
{
	t_eclass *c;

	c = eclass_new("c.breakpoints", (method)breakpoints_new, (method)breakpoints_free, (short)sizeof(t_breakpoints), 0L, A_GIMME, 0);

	eclass_guiinit(c, 0);

    
	eclass_addmethod(c, (method) breakpoints_paint,           "paint",            A_NULL, 0);
	eclass_addmethod(c, (method) breakpoints_notify,          "notify",           A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_getdrawparams,   "getdrawparams",    A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_oksize,          "oksize",           A_NULL, 0);

    eclass_addmethod(c, (method) breakpoints_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) breakpoints_bang,            "bang",             A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_next,            "next",             A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_prev,            "prev",             A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_getlist,         "getlist",          A_NULL, 0);

    eclass_addmethod(c, (method) breakpoints_add,             "add",              A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_move,            "move",             A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_remove,          "remove",           A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_erase,           "erase",            A_NULL,0);
    eclass_addmethod(c, (method) breakpoints_function,        "function",         A_GIMME,0);

    eclass_addmethod(c, (method) breakpoints_scaleabs,        "scaleabs",         A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_scaleord,        "scaleord",         A_GIMME,0);

    eclass_addmethod(c, (method) breakpoints_mousedown,       "mousedown",        A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_mousemove,       "mousemove",        A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_mousedrag,       "mousedrag",        A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_mouseleave,      "mouseleave",       A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_mouseup,         "mouseup",          A_NULL, 0);

    eclass_addmethod(c, (method) breakpoints_preset,          "preset",           A_NULL, 0);
    eclass_addmethod(c, (method) breakpoints_read,            "read",             A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_write,           "write",            A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_save,            "save",             A_NULL, 0);

    CLASS_ATTR_INVISIBLE            (c, "send", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "150. 100.");

    CLASS_ATTR_FLOAT_ARRAY          (c, "absrange", 0, t_breakpoints, f_range_abscissa, 2);
	CLASS_ATTR_ORDER                (c, "absrange", 0, "2");
	CLASS_ATTR_LABEL                (c, "absrange", 0, "Abscissa Range");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "absrange", 0, "0. 1000.");

    CLASS_ATTR_FLOAT_ARRAY          (c, "ordrange", 0, t_breakpoints, f_range_ordinate, 2);
	CLASS_ATTR_ORDER                (c, "ordrange", 0, "2");
	CLASS_ATTR_LABEL                (c, "ordrange", 0, "Ordinate Range");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ordrange", 0, "0. 1.");
    
    CLASS_ATTR_SYMBOL               (c, "outline", 0, t_breakpoints, f_outline);
	CLASS_ATTR_ORDER                (c, "outline", 0, "2");
	CLASS_ATTR_LABEL                (c, "outline", 0, "Outline");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "outline", 0, "Linear");
    CLASS_ATTR_ITEMS                (c, "outline", 0, "Linear Cosine Cubic");
    CLASS_ATTR_STYLE                (c, "outline", 0, "menu");

	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_breakpoints, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_breakpoints, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "ptcolor", 0, t_breakpoints, f_color_point);
	CLASS_ATTR_LABEL                (c, "ptcolor", 0, "Point Color");
	CLASS_ATTR_ORDER                (c, "ptcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "ptcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "licolor", 0, t_breakpoints, f_color_line);
	CLASS_ATTR_LABEL                (c, "licolor", 0, "Line Color");
	CLASS_ATTR_ORDER                (c, "licolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "licolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (c, "licolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_breakpoints, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (c, "textcolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
	breakpoints_class = c;
}







