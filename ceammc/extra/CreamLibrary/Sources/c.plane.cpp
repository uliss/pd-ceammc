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

typedef struct  _plane
{
	t_ebox      j_box;
    t_outlet*   f_out_x;
    t_outlet*   f_out_y;
    float       f_size;
    t_pt        f_position;
    t_rect      f_boundaries;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_point;
} t_plane;

static t_eclass *plane_class;

static void plane_set(t_plane *x, t_symbol *s, int ac, t_atom *av)
{
    if(ac && av)
    {
        if(ac)
        {
            if(x->f_boundaries.x < x->f_boundaries.width)
                x->f_position.x = pd_clip_minmax(atom_getfloat(av), x->f_boundaries.x, x->f_boundaries.width);
            else
                x->f_position.x = pd_clip_minmax(atom_getfloat(av), x->f_boundaries.width, x->f_boundaries.x);
        }
        if(ac > 1)
        {
            if(x->f_boundaries.y < x->f_boundaries.height)
                x->f_position.y = pd_clip_minmax(atom_getfloat(av+1), x->f_boundaries.y, x->f_boundaries.height);
            else
                x->f_position.y = pd_clip_minmax(atom_getfloat(av+1), x->f_boundaries.height, x->f_boundaries.y);
        }
        
        ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void plane_output(t_plane *x)
{
    t_atom argv[2];
    outlet_float(x->f_out_x, x->f_position.x);
    outlet_float(x->f_out_y, x->f_position.y);
    t_pd* send = ebox_getsender((t_ebox *) x);
    if(send)
    {
        atom_setfloat(argv, x->f_position.x);
        atom_setfloat(argv+1, x->f_position.y);
        pd_list(send, &s_list, 2, argv);
    }
}

static void plane_list(t_plane *x, t_symbol *s, int ac, t_atom *av)
{
    plane_set(x, NULL, ac, av);
    plane_output(x);
}

static void plane_getdrawparams(t_plane *x, t_object *patcherview, t_edrawparams *params)
{
    params->d_borderthickness   = 2;
    params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

static void plane_oksize(t_plane *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

static t_pd_err plane_notify(t_plane *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if(msg == cream_sym_attr_modified)
	{
		if(s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_ptcolor || s == cream_sym_ptsize)
		{
			ebox_invalidate_layer((t_ebox *)x, cream_sym_points_layer);
		}
        ebox_redraw((t_ebox *)x);
        
	}
	return 0;
}

static t_pd_err plane_bound_set(t_plane *x, t_object *attr, int ac, t_atom *av)
{
	t_atom argv[2];
    if(ac && av)
    {
        if(ac && atom_gettype(av) == A_FLOAT)
            x->f_boundaries.x = atom_getfloat(av);
        if(ac > 1 && atom_gettype(av+1) == A_FLOAT)
            x->f_boundaries.y = atom_getfloat(av+1);
        if(ac > 2 && atom_gettype(av+2) == A_FLOAT)
            x->f_boundaries.width = atom_getfloat(av+2);
        if(ac > 3 && atom_gettype(av+3) == A_FLOAT)
            x->f_boundaries.height = atom_getfloat(av+3);
        
        if(x->f_boundaries.x == x->f_boundaries.width)
        {
            x->f_boundaries.width += 0.1;
        }
        if(x->f_boundaries.y == x->f_boundaries.height)
        {
            x->f_boundaries.height += 0.1;
        }
        
        atom_setfloat(argv, x->f_position.x);
        atom_setfloat(argv+1, x->f_position.y);
        plane_set(x, NULL, 2, argv);
    }
    return 0;
}

static void plane_paint(t_plane *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    const float ratiox = (rect.width - x->f_size * 2.f) / (x->f_boundaries.width - x->f_boundaries.x);
    const float ratioy = (rect.height - x->f_size * 2.f) / (x->f_boundaries.height - x->f_boundaries.y);
    
    t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_points_layer, rect.width, rect.height);
    if(g && ratiox && ratioy)
    {
        egraphics_set_color_rgba(g, &x->f_color_point);
        egraphics_circle(g,
                         (x->f_position.x - x->f_boundaries.x) * ratiox + x->f_size,
                         rect.height - ((x->f_position.y - x->f_boundaries.y) * ratioy + x->f_size),
                         x->f_size);
        egraphics_fill(g);
        ebox_end_layer((t_ebox*)x, cream_sym_points_layer);
    }
    ebox_paint_layer((t_ebox *)x, cream_sym_points_layer, 0, 0);
    
}

static void plane_mousedrag(t_plane *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_atom argv[2];
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    const float ratiox = (x->f_boundaries.width - x->f_boundaries.x) / (rect.width - x->f_size * 2.f);
    const float ratioy = (x->f_boundaries.height - x->f_boundaries.y) / (rect.height - x->f_size * 2.f);
    if(ratiox && ratioy)
    {
        atom_setfloat(argv, (pt.x - x->f_size) * ratiox + x->f_boundaries.x);
        atom_setfloat(argv+1, (rect.height - pt.y - x->f_size * 0.5f) * ratioy + x->f_boundaries.y);
        plane_set(x, NULL, 2, argv);
        plane_output(x);
    }
}

static void plane_preset(t_plane *x, t_binbuf *b)
{
    binbuf_addv(b, (char *)"sff", &s_list, x->f_position.x, x->f_position.y);
}

static void *plane_new(t_symbol *s, int argc, t_atom *argv)
{
    t_plane *x = (t_plane *)eobj_new(plane_class);
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    if(x && d)
    {
        ebox_new((t_ebox *)x, 0 | EBOX_GROWLINK);
        x->f_out_x = outlet_new((t_object *)x, &s_float);
        x->f_out_y = outlet_new((t_object *)x, &s_float);
        
        x->f_position.x = 0.;
        x->f_position.y = 0.;
        
        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
        return x;
    }
    
    return  NULL;
}

extern "C" void setup_c0x2eplane(void)
{
    t_eclass *c = eclass_new("c.plane", (method)plane_new, (method)ebox_free, (short)sizeof(t_plane), 0L, A_GIMME, 0);
    
    if(c)
    {
        eclass_guiinit(c, 0);
        
        eclass_addmethod(c, (method) plane_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) plane_notify,          "notify",           A_NULL, 0);
        eclass_addmethod(c, (method) plane_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) plane_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) plane_set,             "set",              A_GIMME,0);
        eclass_addmethod(c, (method) plane_list,            "list",             A_GIMME,0);
        eclass_addmethod(c, (method) plane_output,          "bang",             A_NULL, 0);
        eclass_addmethod(c, (method) plane_mousedrag,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) plane_mousedrag,       "mousedrag",        A_NULL, 0);
        eclass_addmethod(c, (method) plane_preset,          "preset",           A_NULL, 0);
        
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        CLASS_ATTR_DEFAULT              (c, "size", 0, "120 120");
        
        CLASS_ATTR_FLOAT_ARRAY          (c, "bound", 0, t_plane, f_boundaries, 4);
        CLASS_ATTR_LABEL                (c, "bound", 0, "Boundaries");
        CLASS_ATTR_ACCESSORS			(c, "bound", NULL, plane_bound_set);
        CLASS_ATTR_ORDER                (c, "bound", 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bound", 0, "-1. -1. 1. 1.");
        
        CLASS_ATTR_FLOAT                (c, "ptsize", 0, t_plane, f_size);
        CLASS_ATTR_LABEL                (c, "ptsize", 0, "Point size");
        CLASS_ATTR_ORDER                (c, "ptsize", 0, "3");
        CLASS_ATTR_FILTER_CLIP          (c, "ptsize", 1., 50.f);
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptsize", 0, "5");
        CLASS_ATTR_STYLE                (c, "ptsize", 0, "number");
        CLASS_ATTR_STEP                 (c, "ptsize", 0.5);
        
        CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_plane, f_color_background);
        CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
        CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_plane, f_color_border);
        CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
        CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
        CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (c, "ptcolor", 0, t_plane, f_color_point);
        CLASS_ATTR_LABEL                (c, "ptcolor", 0, "Point Color");
        CLASS_ATTR_ORDER                (c, "ptcolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptcolor", 0, "0. 0. 0. 1");
        CLASS_ATTR_STYLE                (c, "ptcolor", 0, "color");
        
        eclass_register(CLASS_BOX, c);
        plane_class = c;
    }
}



