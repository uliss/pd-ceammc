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

typedef struct  _prepend
{
    t_eobj      j_box;
    t_outlet*   f_out;
    t_atom*     f_argv;
    int         f_argc;
    t_symbol*   f_selector;
    static const int maxsize = 1024;
} t_prepend;

t_eclass *prepend_class;

void *prepend_new(t_symbol *s, int argc, t_atom *argv);
void prepend_anything(t_prepend *x, t_symbol *s, int argc, t_atom *argv);
void prepend_list(t_prepend *x, t_symbol *s, int argc, t_atom *argv);
void prepend_set(t_prepend *x, t_symbol *s, int argc, t_atom *argv);
void prepend_float(t_prepend *x, float f);
void prepend_symbol(t_prepend *x, t_symbol *s);
void prepend_output(t_prepend *x, long size);
void prepend_free(t_prepend *x);
void prepend_assist(t_prepend *x, void *b, long m, long a, char *s);

void prepend_bang(t_prepend *x);

extern "C" void setup_c0x2eprepend(void)
{
    t_eclass *c;
    
    c = eclass_new("c.prepend", (method)prepend_new, (method)prepend_free, (short)sizeof(t_prepend), 0L, A_GIMME, 0);

    
    eclass_addmethod(c, (method)prepend_anything,    "anything",       A_GIMME, 0);
    eclass_addmethod(c, (method)prepend_list,        "list",           A_GIMME, 0);
    eclass_addmethod(c, (method)prepend_set,         "set",            A_GIMME, 0);
    eclass_addmethod(c, (method)prepend_float,       "float",          A_FLOAT, 0);
    eclass_addmethod(c, (method)prepend_symbol,      "symbol",         A_SYMBOL,0);
    eclass_addmethod(c, (method)prepend_bang,        "bang",           A_NULL,  0);
    eclass_register(CLASS_OBJ, c);
    prepend_class = c;
}

void *prepend_new(t_symbol *s, int argc, t_atom *argv)
{
    t_prepend *x =  NULL;
    
    x = (t_prepend *)eobj_new(prepend_class);
    if(x)
    {
        x->f_argv = (t_atom *)malloc(_prepend::maxsize * sizeof(t_atom));
        prepend_set(x, gensym("set"), argc, argv);
        x->f_out = outlet_new((t_object *)x, &s_list);
    }
    
    return (x);
}

void prepend_set(t_prepend *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_SYMBOL)
        {
            x->f_argc = argc - 1;
            x->f_selector = atom_getsymbol(argv);
            for(i = 0; i < x->f_argc; i++)
            {
                x->f_argv[i] = argv[i+1];
            }
        }
        else
        {
            x->f_argc = argc;
            x->f_selector = &s_list;
            for(i = 0; i < x->f_argc; i++)
            {
                x->f_argv[i] = argv[i];
            }
        }
        
    }
    else
    {
        x->f_argc = 0;
    }
}

void prepend_list(t_prepend *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    if(x->f_argc || x->f_selector != &s_list)
    {
        if(x->f_argc + argc > _prepend::maxsize)
        {
            argc = _prepend::maxsize - x->f_argc;
        }
        for(i = x->f_argc; i < x->f_argc + argc; i++)
        {
            x->f_argv[i] = argv[i - x->f_argc];
        }
        outlet_anything(x->f_out, x->f_selector, x->f_argc + argc, x->f_argv);
    }
    else if(x->f_selector != &s_list)
    {
        outlet_anything(x->f_out, x->f_selector, argc, argv);
    }
    else
    {
        outlet_list(x->f_out, s, argc, argv);
    }
}

void prepend_anything(t_prepend *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    if(x->f_argc || x->f_selector != &s_list)
    {
        if(x->f_argc + argc + 1 > 255)
        {
            argc = 255 - x->f_argc - 1;
        }
        atom_setsym(x->f_argv, s);
        for(i = x->f_argc; i < x->f_argc + argc; i++)
        {
            x->f_argv[i+1] = argv[i - x->f_argc];
        }
        outlet_anything(x->f_out, x->f_selector, x->f_argc + argc + 1, x->f_argv);
    }
    else if(x->f_selector != &s_list)
    {
        if(argc > 255)
            argc = 255;
        atom_setsym(x->f_argv, s);
        for(i = 0; i < argc; i++)
        {
            x->f_argv[i+1] = argv[i];
        }
        outlet_anything(x->f_out, x->f_selector, argc+1, argv);
    }
    else
    {
        outlet_anything(x->f_out, s, argc, argv);
    }
}

void prepend_float(t_prepend *x, float f)
{
    if(x->f_argc < _prepend::maxsize)
    {
        atom_setfloat(x->f_argv+x->f_argc, f);
        outlet_anything(x->f_out, x->f_selector, x->f_argc + 1, x->f_argv);
    }
    else
        outlet_anything(x->f_out, x->f_selector, x->f_argc, x->f_argv);
}

void prepend_symbol(t_prepend *x, t_symbol *s)
{
    if(x->f_argc < _prepend::maxsize)
    {
        atom_setsym(x->f_argv+x->f_argc, s);
        outlet_anything(x->f_out, x->f_selector, x->f_argc + 1, x->f_argv);
    }
    else
        outlet_anything(x->f_out, x->f_selector, x->f_argc, x->f_argv);
}

void prepend_bang(t_prepend *x)
{
    if(x->f_argc < _prepend::maxsize)
    {
        atom_setsym(x->f_argv+x->f_argc, gensym("bang"));
        outlet_anything(x->f_out, x->f_selector, (int)x->f_argc + 1, x->f_argv);
    }
    else
        outlet_anything(x->f_out, x->f_selector, x->f_argc, x->f_argv);
}

void prepend_free(t_prepend *x)
{
    eobj_free(x);
}

