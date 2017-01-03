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

#define MAXITEMS 100

typedef struct  _menu
{
	t_ebox      j_box;
	
    t_outlet*   f_out_index;
    t_outlet*   f_out_item;
    
    t_symbol*   f_items[MAXITEMS];
    long        f_states[MAXITEMS];
    long        f_items_size;
    long        f_states_size;
    long        f_item_selected;
    long        f_hover;
    
    char        f_open;
    float       f_close_height;
    float       f_open_height;
    
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_text;
} t_menu;

t_eclass *menu_class;

void *menu_new(t_symbol *s, int argc, t_atom *argv);
void menu_free(t_menu *x);
void menu_assist(t_menu *x, void *b, long m, long a, char *s);


t_pd_err menu_notify(t_menu *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_pd_err menu_states_set(t_menu *x, t_object *attr, int ac, t_atom *av);
t_pd_err menu_items_set(t_menu *x, t_object *attr, int ac, t_atom *av);
t_pd_err menu_items_get(t_menu *x, t_object *attr, long* ac, t_atom **av);

void menu_append(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_insert(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_setitem(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_delete(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_clear(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_state(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_clean(t_menu *x);

void menu_float(t_menu *x, t_floatarg f);
void menu_symbol(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_set(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_output(t_menu *x);

void menu_getdrawparams(t_menu *x, t_object *patcherview, t_edrawparams *params);
void menu_oksize(t_menu *x, t_rect *newrect);

void menu_paint(t_menu *x, t_object *view);
void draw_background(t_menu *x, t_object *view, t_rect *rect);
void draw_selection(t_menu *x, t_object *view, t_rect *rect);

void menu_mousedown(t_menu *x, t_object *patcherview, t_pt pt, long modifiers);
void menu_mouseleave(t_menu *x, t_object *patcherview, t_pt pt, long modifiers);
void menu_mousemove(t_menu *x, t_object *patcherview, t_pt pt, long modifiers);

void menu_preset(t_menu *x, t_binbuf *b);

extern "C" void setup_c0x2emenu(void)
{
	t_eclass *c;
    
	c = eclass_new("c.menu", (method)menu_new, (method)menu_free, (short)sizeof(t_menu), 0L, A_GIMME, 0);
    
    eclass_guiinit(c, 0);
    eclass_addmethod(c, (method) menu_assist,          "assist",           A_NULL, 0);
	eclass_addmethod(c, (method) menu_paint,           "paint",            A_NULL, 0);
	eclass_addmethod(c, (method) menu_notify,          "notify",           A_NULL, 0);
    eclass_addmethod(c, (method) menu_getdrawparams,   "getdrawparams",    A_NULL, 0);
    eclass_addmethod(c, (method) menu_oksize,          "oksize",           A_NULL, 0);
    
    eclass_addmethod(c, (method) menu_append,          "append",           A_GIMME,0);
    eclass_addmethod(c, (method) menu_insert,          "insert",           A_GIMME,0);
    eclass_addmethod(c, (method) menu_setitem,         "setitem",          A_GIMME,0);
    eclass_addmethod(c, (method) menu_delete,          "delete",           A_GIMME,0);
    eclass_addmethod(c, (method) menu_clear,           "clear",            A_GIMME,0);
    eclass_addmethod(c, (method) menu_state,           "state",            A_GIMME,0);
    
    eclass_addmethod(c, (method) menu_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) menu_symbol,          "anything",         A_GIMME,0);
    eclass_addmethod(c, (method) menu_set,             "set",              A_GIMME,0);
    eclass_addmethod(c, (method) menu_output,          "bang",             A_NULL, 0);
    
    eclass_addmethod(c, (method) menu_mousedown,        "mousedown",       A_NULL, 0);
    eclass_addmethod(c, (method) menu_mousemove,        "mousemove",       A_NULL, 0);
    eclass_addmethod(c, (method) menu_mouseleave,       "mouseleave",      A_NULL, 0);
    eclass_addmethod(c, (method) menu_preset,           "preset",          A_NULL, 0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "100 13");
    
    CLASS_ATTR_LONG                 (c, "hover", 0, t_menu, f_hover);
	CLASS_ATTR_LABEL                (c, "hover", 0, "Hover Mode");
	CLASS_ATTR_ORDER                (c, "hover", 0, "1");
    CLASS_ATTR_FILTER_CLIP          (c, "hover", 0, 1);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "hover", 0, "0");
    CLASS_ATTR_STYLE                (c, "hover", 0, "onoff");
    
    CLASS_ATTR_SYMBOL_VARSIZE       (c, "items", 0, t_menu, f_items, f_items_size, MAXITEMS);
    CLASS_ATTR_LABEL                (c, "items", 0, "Items");
    CLASS_ATTR_ACCESSORS            (c, "items", menu_items_get, menu_items_set);
    CLASS_ATTR_ORDER                (c, "items", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "items", 0, "(null)");
    
    CLASS_ATTR_LONG_VARSIZE         (c, "states", 0, t_menu, f_states, f_states_size, MAXITEMS);
    CLASS_ATTR_LABEL                (c, "states", 0, "Items Disable State");
    CLASS_ATTR_ACCESSORS            (c, "states", NULL, menu_states_set);
    CLASS_ATTR_ORDER                (c, "states", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "states", 0, "0");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_menu, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_menu, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_menu, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
	CLASS_ATTR_STYLE                (c, "textcolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
	menu_class = c;
}

void *menu_new(t_symbol *s, int argc, t_atom *argv)
{
	t_menu *x =  NULL;
	t_binbuf* d;
    long flags;
    
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_menu *)eobj_new(menu_class);
    
    flags = 0
    | EBOX_GROWINDI
    ;
    
	ebox_new((t_ebox *)x, flags);
    
    x->f_out_index  = outlet_new((t_object *)x, &s_float);
    x->f_out_item   = outlet_new((t_object *)x, &s_list);
    x->f_item_selected = 0;
    x->f_items_size = 0;
    x->f_open       = 0;
    
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
    
	return (x);
}

void menu_free(t_menu *x)
{
	ebox_free((t_ebox *)x);
}

void menu_assist(t_menu *x, void *b, long m, long a, char *s)
{
	;
}

// MENU GESTION //

static t_symbol* menu_atoms_to_sym(t_atom* argv, long argc)
{
    int i;
    size_t length;
    char temp[MAXPDSTRING];
    char text[MAXPDSTRING];
    atom_string(argv, text, MAXPDSTRING);
    for(i = 1; i < argc; i++)
    {
        atom_string(argv+i, temp, MAXPDSTRING);
        length = strlen(temp);
        strncat(text, " ", 1);
        strncat(text, temp, length);
    }
    return gensym(text);
}

static long menu_symbol_exist(t_menu *x, t_symbol* s)
{
    long i;
    long j = -1;
    for(i = 0; i < x->f_items_size; i++)
    {
        if(!strcmp(s->s_name, x->f_items[i]->s_name))
        {
            j = i;
            break;
        }
    }
    return j;
}

void menu_append(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol* item = menu_atoms_to_sym(argv, argc);
    
    if(argc && argv && menu_symbol_exist(x, item) == -1)
    {
        x->f_items[x->f_items_size] = item;
        x->f_items_size++;
        
        ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
        
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

void menu_insert(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    t_symbol* item = menu_atoms_to_sym(argv+1, argc-1);
    
    if(argc > 1 && argv && atom_gettype(argv) == A_FLOAT && atom_getfloat(argv) >= 0 && menu_symbol_exist(x, item) == -1)
    {
        if(atom_getfloat(argv) >= x->f_items_size)
            x->f_items[x->f_items_size] = item;
        else
        {
            for(i = (int)x->f_items_size - 1; i >= atom_getfloat(argv); i--)
                x->f_items[i+1] = x->f_items[i];
            x->f_items[atom_getint(argv)] = item;
        }
        x->f_items_size++;
        
        ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
        
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

void menu_setitem(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol* item = menu_atoms_to_sym(argv+1, argc-1);
    if(argc > 1 && argv && atom_gettype(argv) == A_FLOAT && menu_symbol_exist(x, item) == -1)
    {
        if(atom_getfloat(argv) >= 0 && atom_getfloat(argv) < x->f_items_size)
            x->f_items[atom_getint(argv)] = item;
        
        ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
        
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

void menu_delete(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    if(argc > 0 && argv && atom_gettype(argv) == A_FLOAT)
    {
        if(atom_getfloat(argv) >= 0 && atom_getfloat(argv) < x->f_items_size)
        {
            for(i = atom_getfloat(argv); i < x->f_items_size - 1; i++)
                x->f_items[i] = x->f_items[i+1];
            x->f_items_size--;
            for(i = (int)x->f_items_size; i < MAXITEMS; i++)
                x->f_items[i] = s_null;
            
            ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
            
            ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
            ebox_redraw((t_ebox *)x);
        }
    }
}

void menu_clear(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    for(i = 0; i < MAXITEMS; i++)
    {
        x->f_items[i] = s_null;
    }
    x->f_items_size = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
    
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

void menu_state(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    if(argc > 1 && argv && atom_gettype(argv) == A_FLOAT)
    {
        if(atom_getfloat(argv) >= 0 && atom_getfloat(argv) < x->f_items_size)
        {
            if(atom_gettype(argv+1) == A_FLOAT && atom_getfloat(argv+1) != 0)
                x->f_states[(int)atom_getfloat(argv)] = 1;
            else
                x->f_states[(int)atom_getfloat(argv)] = 0;
            
            ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
            
            ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
            ebox_redraw((t_ebox *)x);
        }
    }
}

void menu_clean(t_menu *x)
{
    int i, j;
    for(i = 0; i < x->f_items_size; i++)
    {
        if(x->f_items[i] == s_null)
        {
            for(j = i; j < x->f_items_size; j++)
                x->f_items[j] = x->f_items[j+1];
            x->f_items_size--;
            i--;
        }
    }
    x->f_states_size = x->f_items_size;
}

// MENU SELECTION AND OUTPUT

static void menu_setfloat(t_menu *x, t_floatarg f)
{
    if(f >= 0 && f < x->f_items_size)
    {
        x->f_item_selected = f;
        ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
        
        ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
        ebox_redraw((t_ebox *)x);
    }
}

static void menu_setsymbol(t_menu *x, t_symbol* s)
{
    long i = menu_symbol_exist(x, s);
    if(i != -1)
        x->f_item_selected = i;
    ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
    
    ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
    ebox_redraw((t_ebox *)x);
}

void menu_float(t_menu *x, t_floatarg f)
{
    menu_setfloat(x, f);
    menu_output(x);
}

void menu_symbol(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    t_atom* av = (t_atom *)calloc((size_t)(argc + 1), sizeof(t_atom));
    atom_setsym(av, s);
    for(i = 0; i < argc; i++)
        av[i+1] = argv[i];

    menu_setsymbol(x, menu_atoms_to_sym(av, argc+1));
    menu_output(x);
}

void menu_set(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_FLOAT)
            menu_setfloat(x, atom_getfloat(argv));
        else if (atom_gettype(argv) == A_SYMBOL)
            menu_setsymbol(x, menu_atoms_to_sym(argv, argc));
    }
}

void menu_output(t_menu *x)
{
    if(x->f_items_size > 0)
    {
        outlet_float(x->f_out_index, x->f_item_selected);
        if(ebox_getsender((t_ebox *) x))
            pd_float(ebox_getsender((t_ebox *) x), (float)x->f_item_selected);
        outlet_symbol(x->f_out_item, x->f_items[x->f_item_selected]);
    }
}

// MENU DRAW

void menu_getdrawparams(t_menu *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void menu_oksize(t_menu *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.b_font.c_size) * 3 + 8);
    x->f_close_height = newrect->height = sys_fontheight(x->j_box.b_font.c_size) + 4;
    if(newrect->width < newrect->height * 2)
        newrect->width = newrect->height * 2;
    
    if(x->f_open)
        newrect->height *= (x->f_items_size + 1);
    
}

t_pd_err menu_notify(t_menu *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == cream_sym_attr_modified)
	{
		if(s == cream_sym_bgcolor || s == cream_sym_bdcolor || s == cream_sym_textcolor || s == cream_sym_fontsize || s == cream_sym_fontname || s == cream_sym_fontweight || s == cream_sym_fontslant || s == gensym("states"))
		{
            ebox_invalidate_layer((t_ebox *)x, gensym("list_layer"));
			ebox_invalidate_layer((t_ebox *)x, cream_sym_background_layer);
		}
        if(s == cream_sym_fontsize || s == gensym("items"))
        {
            eobj_attr_setvalueof(x, gensym("size"), 0, NULL);
        }
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void menu_paint(t_menu *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    menu_clean(x);
 
    draw_background(x, view, &rect);
    if(x->f_open)
    {
        draw_selection(x, view, &rect);
    }
    
}

void draw_background(t_menu *x, t_object *view, t_rect *rect)
{
    float width;
	t_elayer *g = ebox_start_layer((t_ebox *)x, cream_sym_background_layer, rect->width, rect->height);
    t_etext *jtl = etext_layout_create();
	if(g && jtl)
	{
        
        if(x->f_items_size == 0 || (x->f_items_size == 1 && x->f_items[0] == s_null))
        {
            ;
        }
        else
        {
            etext_layout_set(jtl, x->f_items[x->f_item_selected]->s_name, &x->j_box.b_font, 1.5, x->f_close_height / 2. + 1, rect->width, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_settextcolor(jtl, &x->f_color_text);
            etext_layout_draw(jtl, g);
        }
        
        // Right - Erase text
        width = rect->width - x->f_close_height;
        egraphics_set_color_rgba(g, &x->f_color_background);
        egraphics_rectangle(g, width, 0., rect->width, x->f_close_height);
        egraphics_fill(g);
        
        // Separation
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2);
        egraphics_line_fast(g, width, 0., width, x->f_close_height);
        
        // Arraw Up
        egraphics_move_to(g, width + x->f_close_height * 0.3 + 1, x->f_close_height * 0.4);
        egraphics_line_to(g, width + x->f_close_height * 0.7 + 1, x->f_close_height * 0.4);
        egraphics_line_to(g, width + x->f_close_height * 0.5 + 1, x->f_close_height * 0.1);
        egraphics_fill(g);
        
        // Arraw Down
        egraphics_move_to(g, width + x->f_close_height * 0.3 + 1, x->f_close_height * 0.6);
        egraphics_line_to(g, width + x->f_close_height * 0.7 + 1, x->f_close_height * 0.6);
        egraphics_line_to(g, width + x->f_close_height * 0.5 + 1, x->f_close_height * 0.9);
        egraphics_fill(g);
        
		ebox_end_layer((t_ebox*)x, cream_sym_background_layer);
	}
    etext_layout_destroy(jtl);
	ebox_paint_layer((t_ebox *)x, cream_sym_background_layer,  0., 0.);
}

void draw_selection(t_menu *x, t_object *view, t_rect *rect)
{
    int i;
    float width;
    t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("list_layer"), rect->width, rect->height);
    t_etext *jtl = etext_layout_create();
	if (g && jtl)
	{
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 1);
        
        for(i = 0; i < x->f_items_size; i++)
        {
            egraphics_line_fast(g, 0., x->f_close_height * (i + 1), rect->width - x->f_close_height, x->f_close_height * (i + 1));
            if(x->f_items[i] != s_null)
            {
                if(x->f_states[i])
                    etext_layout_settextcolor(jtl, &x->f_color_border);
                else
                    etext_layout_settextcolor(jtl, &x->f_color_text);
                etext_layout_set(jtl, x->f_items[i]->s_name, &x->j_box.b_font, 1.5, x->f_close_height / 2. + x->f_close_height * (i + 1) + 2, rect->width, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
                etext_layout_draw(jtl, g);
            }
        }
        
        egraphics_set_color_rgba(g, &x->f_color_background);
        width = rect->width - x->f_close_height;
        egraphics_rectangle(g, width, x->f_close_height, rect->width, rect->height);
        egraphics_fill(g);
        
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_line_fast(g, width, 0., width, rect->height);
        
        egraphics_set_line_width(g, 2);
        egraphics_line_fast(g, 0., x->f_close_height, rect->width, x->f_close_height);
        
        if(x->f_items_size > x->f_item_selected &&  x->f_items[x->f_item_selected] != s_null)
        {
            egraphics_circle(g, rect->width - x->f_close_height * 0.5 + 1.5, x->f_close_height * (x->f_item_selected + 1.5), x->f_close_height * 0.25);
            egraphics_stroke(g);
        }
        
		ebox_end_layer((t_ebox*)x, gensym("list_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("list_layer"), 0., 0.);
}

void menu_mousedown(t_menu *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int index;
    t_atom av[1];
    if(x->f_open == 1 && pt.y > x->f_close_height)
    {
        index = (pt.y - x->f_close_height) / (x->f_close_height);
        if(index >= 0 && index < x->f_items_size && x->f_states[index] == 0)
        {
            menu_float(x, index);
        }
        
    }
    if(!x->f_hover)
    {
        if(x->f_open)
            x->f_open = 0;
        else
            x->f_open = 1;
        atom_setfloat(av, x->j_box.b_rect.width);
        eobj_attr_setvalueof(x, gensym("size"), 1, av);
    }
}

void menu_mouseleave(t_menu *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_atom av[1];
    atom_setfloat(av, x->j_box.b_rect.width);
    x->f_open = 0;
    eobj_attr_setvalueof(x, gensym("size"), 1, av);
}

void menu_mousemove(t_menu *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_atom av[1];
    if(x->f_hover && x->f_open != 1)
    {
        atom_setfloat(av, x->j_box.b_rect.width);
        x->f_open = 1;
        eobj_attr_setvalueof(x, gensym("size"), 1, av);
    }
}

t_pd_err menu_states_set(t_menu *x, t_object *attr, int ac, t_atom *av)
{
    int i;
    if(ac && av)
    {
        x->f_states_size = x->f_items_size;
        for(i = 0; i < ac && i < x->f_items_size; i++)
        {
            if(atom_gettype(av+i) == A_FLOAT && atom_getfloat(av+i) != 0)
                x->f_states[i] = 1;
            else
                x->f_states[i] = 0;
        }
    }
    return 0;
}

void menu_preset(t_menu *x, t_binbuf *b)
{
    binbuf_addv(b, (char *)"sf", gensym("float"), (float)x->f_item_selected);
}

t_pd_err menu_items_set(t_menu *x, t_object *attr, int ac, t_atom *av)
{
    char text[MAXPDSTRING];
    menu_clear(x, NULL, 0, NULL);

    if(ac && av)
    {
        for(int i = 0; i < ac; i++)
        {
            atom_string(av+i, text, MAXPDSTRING);
            x->f_items[i] = gensym(text);
        }
        
    }
    
    x->f_items_size = ac;
    return 0;
}

t_pd_err menu_items_get(t_menu *x, t_object *attr, long* ac, t_atom **av)
{
    int i;
    *ac = x->f_items_size;
    av[0] = (t_atom *)calloc((size_t)(*ac), sizeof(t_atom));
    for(i = 0; i < *ac; i++)
    {
        atom_setsym(av[0]+i, x->f_items[i]);
    }
    
    return 0;
}



