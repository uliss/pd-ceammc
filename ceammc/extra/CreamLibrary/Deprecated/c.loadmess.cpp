/*
 * PdEnhanced - Pure Data Enhanced
 *
 * An add-on for Pure Data
 *
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "../c.library.hpp"

typedef struct  _loadmess
{
	t_eobj      l_box;
	t_outlet*   l_out;
    t_atom*     l_argv;
	size_t      l_argc;
    double      l_time;
} t_loadmess;

t_eclass *loadmess_class;

static void loadmess_output(t_loadmess *x)
{
    if(!x->l_argc)
    {
        outlet_bang(x->l_out);
    }
    else if(x->l_argc == 1)
    {
        if(atom_gettype(x->l_argv) == A_FLOAT)
            outlet_float(x->l_out, atom_getfloat(x->l_argv));
        else if (atom_gettype(x->l_argv) == A_SYMBOL)
            outlet_symbol(x->l_out, atom_getsymbol(x->l_argv));
    }
    else
    {
        if(atom_gettype(x->l_argv) == A_FLOAT)
            outlet_list(x->l_out, &s_list, (int)x->l_argc, x->l_argv);
        else if (atom_gettype(x->l_argv) == A_SYMBOL)
            outlet_anything(x->l_out, atom_getsymbol(x->l_argv), (int)x->l_argc-1, x->l_argv+1);
    }
}

static void loadmess_loadbang(t_loadmess *x)
{
    loadmess_output(x);
}

static void loadmess_click(t_loadmess *x)
{
    if(clock_gettimesince(x->l_time) < 250.)
        loadmess_output(x);
    x->l_time = clock_getsystime();
}

static void loadmess_free(t_loadmess *x)
{
    if(x->l_argv)
    {
        free(x->l_argv);
    }
    eobj_free(x);
}

static void *loadmess_new(t_symbol *s, int argc, t_atom *argv)
{
    t_loadmess *x = (t_loadmess *)eobj_new(loadmess_class);
    if(x)
    {
        x->l_time = clock_getsystime();
        if(argc && argv)
        {
            x->l_argc = (size_t)argc;
            x->l_argv = (t_atom *)malloc(x->l_argc * sizeof(t_atom));
            if(x->l_argv)
            {
                memcpy(x->l_argv, argv, sizeof(t_atom) * x->l_argc);
                if(x->l_argc == 1)
                {
                    if(atom_gettype(argv) == A_FLOAT)
                        x->l_out = outlet_new((t_object *)x, &s_float);
                    else if (atom_gettype(argv) == A_SYMBOL)
                        x->l_out = outlet_new((t_object *)x, &s_symbol);
                }
                else
                {
                    if(atom_gettype(argv) == A_FLOAT)
                        x->l_out = outlet_new((t_object *)x, &s_list);
                    else if (atom_gettype(argv) == A_SYMBOL)
                        x->l_out = outlet_new((t_object *)x, &s_anything);
                }
            }
            else
            {
                pd_error(x, "can't allocate memory.");
            }
        }
        else
        {
            x->l_argc = 0;
            x->l_argv = NULL;
            x->l_out = outlet_new((t_object *)x, &s_bang);
        }
    }
    
    return (x);
}

extern "C" void setup_c0x2eloadmess(void)
{
	t_eclass *c;
    
	c = eclass_new("c.loadmess", (method)loadmess_new, (method)loadmess_free, (short)sizeof(t_loadmess), 0L, A_GIMME, 0);

    eclass_addmethod(c, (method) loadmess_loadbang,    "loadbang",         A_NULL, 0);
    eclass_addmethod(c, (method) loadmess_output,      "bang",             A_NULL, 0);
	eclass_addmethod(c, (method) loadmess_click,       "click",            A_NULL, 0);
	eclass_register(CLASS_OBJ, c);
    loadmess_class = c;
}


