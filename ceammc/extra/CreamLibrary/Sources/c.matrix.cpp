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

typedef struct  _matrixctrl
{
	t_ebox      j_box;

    t_outlet*   f_out_cell;
    t_outlet*   f_out_colrow;
    char**      f_values;
    int         f_size[2];
    int         f_selected[2];
	t_rgba		f_color_background;
	t_rgba		f_color_border;
    t_rgba      f_color_on;

} t_matrixctrl;

static t_eclass *matrixctrl_class;

static void matrixctrl_output(t_matrixctrl *x, const int i, const int j)
{
    t_atom av[3];
    if(i < x->f_size[0] && j < x->f_size[1])
    {
        atom_setfloat(av, i);
        atom_setfloat(av+1, j);
        atom_setfloat(av+2,  x->f_values[i][j]);
        
        t_pd* send = ebox_getsender((t_ebox *) x);
        outlet_list(x->f_out_cell, &s_list, 3, av);
        if(send)
        {
            pd_list(ebox_getsender((t_ebox *) x), &s_list, 3, av);
        }
    }
}

static void matrixctrl_bang(t_matrixctrl *x)
{
    for(int i = 0; i < (int)x->f_size[1]; i++)
    {
        for(int j = 0; j < (int)x->f_size[0]; j++)
        {
            matrixctrl_output(x, (int)j, (int)i);
        }
    }
}

static void matrixctrl_clear(t_matrixctrl *x)
{
    for(int i = 0; i < x->f_size[1]; i++)
    {
        for(int j = 0; j < x->f_size[0]; j++)
        {
           if(x->f_values[i][j])
           {
               x->f_values[i][j] = 0;
               matrixctrl_output(x, j, i);
           }
        }
    }
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

static void matrixctrl_getrow(t_matrixctrl *x, float f)
{
    t_atom* av;
    if(f >= 0 && f < x->f_size[1])
    {
        av = (t_atom *)malloc((size_t)x->f_size[0] * sizeof(t_atom));
        if(av)
        {
            for(int i = 0; i < x->f_size[0]; i++)
            {
                atom_setfloat(av+i, (float)x->f_values[i][(int)f]);
            }
            outlet_list(x->f_out_colrow, &s_list, (int)x->f_size[0], av);
            free(av);
        }
    }
}

static void matrixctrl_getcolumn(t_matrixctrl *x, float f)
{
    t_atom* av;
    if(f >= 0 && f < x->f_size[0])
    {
        av = (t_atom *)malloc((size_t)x->f_size[1] * sizeof(t_atom));
        if(av)
        {
            for(int i = 0; i < x->f_size[1]; i++)
            {
                atom_setfloat(av+i, x->f_values[(int)f][i]);
            }
            outlet_list(x->f_out_colrow, &s_list, (int)x->f_size[1], av);
            free(av);
        }
    }
}

static void matrixctrl_set(t_matrixctrl *x, t_symbol *s, int ac, t_atom *av)
{
    if(ac && av)
    {
        for(int i = 2; i < ac; i += 3)
        {
            if(atom_gettype(av+i-2) == A_FLOAT && atom_gettype(av+i-1) == A_FLOAT && atom_gettype(av+i) == A_FLOAT)
            {
                int column  = atom_getfloat(av+i-2);
                int row     = atom_getfloat(av+i-1);
                char value  = atom_getfloat(av+i);
                if(column < x->f_size[0] && row < x->f_size[1])
                {
                    x->f_values[row][column] = value;
                }
            }
        }

        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void matrixctrl_list(t_matrixctrl *x, t_symbol *s, int ac, t_atom *av)
{
    if(ac && av)
    {
        for(long i = 2; i < ac; i += 3)
        {
            if(atom_gettype(av+i-2) == A_FLOAT && atom_gettype(av+i-1) == A_FLOAT && atom_gettype(av+i) == A_FLOAT)
            {
                int column  = atom_getfloat(av+i-2);
                int row     = atom_getfloat(av+i-1);
                char value  = atom_getfloat(av+i);
                if(column < x->f_size[0] && row < x->f_size[1])
                {
                    x->f_values[row][column] = value;
                    matrixctrl_output(x, column, row);
                }
            }
        }

        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void matrixctrl_getdrawparams(t_matrixctrl *x, t_object *patcherview, t_edrawparams *params)
{
    params->d_borderthickness   = 2;
    params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

static void matrixctrl_oksize(t_matrixctrl *x, t_rect *newrect)
{
    float ratio;
    newrect->width = pd_clip_min(newrect->width, 30.);
    newrect->height = pd_clip_min(newrect->height, 10.);
    
    ratio = (newrect->width - 1.) / (float)x->f_size[0];
    if(ratio - (int)ratio != 0)
    {
        ratio = floorf(ratio);
        newrect->width = ratio * (float)x->f_size[0] + 1.;
    }
    ratio = (newrect->height - 1.) / (float)x->f_size[1];
    if(ratio - (int)ratio != 0)
    {
        ratio = floorf(ratio);
        newrect->height = ratio * (float)x->f_size[1] + 1.;
    }
    
    newrect->width = pd_clip_min(newrect->width, 30.);
    newrect->height = pd_clip_min(newrect->height, 10.);
}

static t_pd_err matrixctrl_notify(t_matrixctrl *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if(msg == cream_sym_attr_modified)
	{
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
	}
	return 0;
}

static void matrixctrl_paint(t_matrixctrl *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect.width, rect.height);
    if(g)
    {
        const int block_width = rect.width / x->f_size[0];
        const int block_height = rect.height / x->f_size[1];
        for(int incx = 0, i = 0; i < x->f_size[0]; i++, incx += block_width)
        {
            for(int incY = 0, j = 0; j < x->f_size[1]; j++, incY += block_height)
            {
                egraphics_rectangle_rounded(g, incx + 1.f, incY + 1.f, block_width - 2.f, block_height - 2.f, 1.f);
                if(x->f_values[i][j])
                {
                    egraphics_set_color_rgba(g, &x->f_color_on);
                    egraphics_fill_preserve(g);
                }
                egraphics_set_color_rgba(g, &x->f_color_border);
                egraphics_stroke(g);
            }
        }
        
        ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_background_layer, 0, 0);
}

static void matrixctrl_mousedown(t_matrixctrl *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_selected[0] = (int)pd_clip_minmax(pt.x / (rect.width / (float)x->f_size[0]), 0.f, (float)x->f_size[0] - 1.f);
    x->f_selected[1] = (int)pd_clip_minmax(pt.y / (rect.height / (float)x->f_size[1]), 0.f, (float)x->f_size[1] - 1.f);
    if(x->f_selected[0] >= 0 && x->f_selected[0] < x->f_size[0] && x->f_selected[1] >= 0 && x->f_selected[1] < x->f_size[1])
    {
        x->f_values[x->f_selected[0]][x->f_selected[1]] = !x->f_values[x->f_selected[0]][x->f_selected[1]];
        matrixctrl_output(x, x->f_selected[0], x->f_selected[1]);
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void matrixctrl_mousedrag(t_matrixctrl *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    int newpt[2];
    newpt[0] = (int)pd_clip_minmax(pt.x / (rect.width / (float)x->f_size[0]), 0.f, (float)x->f_size[0] - 1.f);
    newpt[1] = (int)pd_clip_minmax(pt.y / (rect.height / (float)x->f_size[1]), 0.f, (float)x->f_size[1] - 1.f);
    if(newpt[0] != x->f_selected[0] || newpt[1] != x->f_selected[1])
    {
        x->f_selected[0] = newpt[0];
        x->f_selected[1] = newpt[1];
        if(x->f_selected[0] >= 0 && x->f_selected[0] < x->f_size[0] && x->f_selected[1] >= 0 && x->f_selected[1] < x->f_size[1])
        {
            x->f_values[x->f_selected[0]][x->f_selected[1]] = !x->f_values[x->f_selected[0]][x->f_selected[1]];
            matrixctrl_output(x, x->f_selected[0], x->f_selected[1]);
            ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
            ebox_redraw((t_ebox *)x);
        }
    }
}

static void matrixctrl_mouseleave(t_matrixctrl *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_selected[0] = -1;
    x->f_selected[1] = -1;
}

static void matrixctrl_preset(t_matrixctrl *x, t_binbuf *b)
{
    t_atom* av = (t_atom *)malloc((size_t)(x->f_size[0] * x->f_size[1] * 3) * sizeof(t_atom));
    int ac = 0;
    if(av)
    {
        for(int i = 0; i < (int)x->f_size[1]; i++)
        {
            for(int j = 0; j < (int)x->f_size[0]; j++)
            {
                atom_setfloat(av+ac, j);
                atom_setfloat(av+ac+1, i);
                atom_setfloat(av+ac+2, (float)x->f_values[i][j]);
                ac += 3;
            }
        }
        
        binbuf_addv(b, (char *)"s", &s_list);
        binbuf_add(b, x->f_size[0] * x->f_size[1] * 3, av);
        free(av);
    }
}

static t_pd_err matrixctrl_matrix_set(t_matrixctrl *x, t_object *attr, int ac, t_atom *av)
{
    if(ac > 1 && av && atom_gettype(av) == A_FLOAT && atom_gettype(av+1) == A_FLOAT)
    {
        x->f_size[0] = (int)pd_clip_minmax(atom_getfloat(av), 1, 256);
        x->f_size[1] = (int)pd_clip_minmax(atom_getfloat(av+1), 1, 256);
        for(int i = 0; i < 256; i++)
        {
            x->f_values[i] = (char *)malloc(256 * sizeof(char));
            memset(x->f_values[i], 0, 256 * sizeof(char));
        }
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
    return 0;
}

static void *matrixctrl_new(t_symbol *s, int argc, t_atom *argv)
{
    t_matrixctrl *x = (t_matrixctrl *)eobj_new(matrixctrl_class);
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    
    if(x && d)
    {
        x->f_size[0] = 8;
        x->f_size[1] = 4;
        x->f_selected[0] = -1;
        x->f_selected[1] = -1;
        x->f_values = (char **)malloc(256 * sizeof(char *));
        for(int i = 0; i < 256; i++)
        {
            x->f_values[i] = (char *)malloc(256 * sizeof(char));
            memset(x->f_values[i], 0, 256 * sizeof(char));
        }
        ebox_new((t_ebox *)x, 0 | EBOX_GROWINDI);
        
        x->f_out_cell   = outlet_new((t_object *)x, &s_list);
        x->f_out_colrow = outlet_new((t_object *)x, &s_list);
        
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
        
        return x;
    }
    
    return NULL;
}

static void matrixctrl_free(t_matrixctrl *x)
{
    ebox_free((t_ebox *)x);
    free(x->f_values);
}

extern "C" void setup_c0x2ematrix(void)
{
    t_eclass *c = eclass_new("c.matrix", (method)matrixctrl_new, (method)matrixctrl_free, (short)sizeof(t_matrixctrl), 0L, A_GIMME, 0);
    
    if(c)
    {
        eclass_guiinit(c, 0);
        eclass_addmethod(c, (method) matrixctrl_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_notify,          "notify",           A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_set,             "set",              A_GIMME,0);
        eclass_addmethod(c, (method) matrixctrl_list,            "list",             A_GIMME,0);
        eclass_addmethod(c, (method) matrixctrl_bang,            "bang",             A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_clear,           "clear",            A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_getrow,          "getrow",           A_FLOAT,0);
        eclass_addmethod(c, (method) matrixctrl_getcolumn,       "getcolumn",        A_FLOAT,0);
        
        eclass_addmethod(c, (method) matrixctrl_mousedown,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_mousedrag,       "mousedrag",        A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_mouseleave,      "mouseleave",       A_NULL, 0);
        
        eclass_addmethod(c, (method) matrixctrl_preset,          "preset",           A_NULL, 0);
        
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        CLASS_ATTR_DEFAULT              (c, "size", 0, "105 53");
        
        CLASS_ATTR_INT_ARRAY            (c, "matrix", 0, t_matrixctrl, f_size, 2);
        CLASS_ATTR_LABEL                (c, "matrix", 0, "Matrix Size");
        CLASS_ATTR_ACCESSORS			(c, "matrix", NULL, matrixctrl_matrix_set);
        CLASS_ATTR_ORDER                (c, "matrix", 0, "1");
        CLASS_ATTR_DEFAULT              (c, "matrix", 0, "8 4");
        CLASS_ATTR_SAVE                 (c, "matrix", 0);
        
        CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_matrixctrl, f_color_background);
        CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
        CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_matrixctrl, f_color_border);
        CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
        CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "accolor", 0, t_matrixctrl, f_color_on);
        CLASS_ATTR_LABEL                (c, "accolor", 0, "Active Cell Color");
        CLASS_ATTR_ORDER                (c, "accolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "accolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "accolor", 0, "color");
        
        eclass_register(CLASS_BOX, c);
        matrixctrl_class = c;
    }
}




