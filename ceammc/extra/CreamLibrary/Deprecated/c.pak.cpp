/*
 * PdEnhanced - Pure Data Enhanced
 *
 * An add-on for Pure Data
 *
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CreamLibrary
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "../c.library.hpp"

typedef struct  _pak
{
	t_eobj      j_box;
	t_outlet*   f_out;
    t_atom*     f_argv;
	int         f_argc;
    char*       f_selectors;
} t_pak;

t_eclass *pak_class;

static void pak_output(t_pak *x)
{
    outlet_list(x->f_out, &s_list, (int)x->f_argc, x->f_argv);
}

static void pak_list(t_pak *x, t_symbol *s, int argc, t_atom *argv)
{
    int index = eobj_getproxy((t_ebox *)x);
    if(argc && x->f_selectors[index] == 0 && atom_gettype(argv) == A_FLOAT)
    {
        atom_setfloat(x->f_argv+index, atom_getfloat(argv));
        pak_output(x);
    }
    else if(argc && x->f_selectors[index] == 1 && atom_gettype(argv) == A_SYMBOL)
    {
        atom_setsym(x->f_argv+index, atom_getsymbol(argv));
        pak_output(x);
    }
}

static void pak_float(t_pak *x, float f)
{
    int index = eobj_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 0)
    {
        atom_setfloat(x->f_argv+index, f);
        pak_output(x);
    }
}

static void pak_anything(t_pak *x, t_symbol *s, int argc, t_atom *argv)
{
    int index = eobj_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 1)
    {
        atom_setsym(x->f_argv+index, s);
        pak_output(x);
    }
}

static void pak_symbol(t_pak *x, t_symbol *s)
{
    int index = eobj_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 1)
    {
        atom_setsym(x->f_argv+index, s);
        pak_output(x);
    }
}

static void pak_free(t_pak *x)
{
    eobj_free(x);
    free(x->f_selectors);
    free(x->f_argv);
}

static void *pak_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
    t_pak *x = (t_pak *)eobj_new(pak_class);
    if(x)
    {
        if(argc < 2)
        {
            argc = 2;
        }
        x->f_argc = argc;
        x->f_argv = (t_atom *)calloc((size_t)x->f_argc, sizeof(t_atom));
        x->f_selectors = (char *)calloc((size_t)x->f_argc, sizeof(char));
        eobj_proxynew(x);
        eobj_proxynew(x);
        if(argc > 0 &&atom_gettype(argv) == A_SYMBOL && (atom_getsymbol(argv) == gensym("f") || atom_getsymbol(argv) == gensym("float")))
        {
            x->f_selectors[0] = 0;
            atom_setfloat(x->f_argv, 0.);
        }
        else if(argc > 0 && atom_gettype(argv) == A_FLOAT)
        {
            x->f_selectors[0] = 0;
            atom_setfloat(x->f_argv, atom_getfloat(argv));
        }
        else if(argc > 0 && atom_gettype(argv) == A_SYMBOL && (atom_getsymbol(argv) == gensym("s") || atom_getsymbol(argv) == gensym("symbol")))
        {
            x->f_selectors[0] = 1;
            atom_setsym(x->f_argv, gensym("symbol"));
        }
        else if(argc > 0 && atom_gettype(argv) == A_SYMBOL)
        {
            x->f_selectors[0] = 1;
            atom_setsym(x->f_argv, atom_getsymbol(argv));
        }
        else
        {
            x->f_selectors[0] = 0;
            atom_setfloat(x->f_argv, 0.);
        }
        if(argc > 1 && atom_gettype(argv+1) == A_SYMBOL && (atom_getsymbol(argv+1) == gensym("f") || atom_getsymbol(argv+1) == gensym("float")))
        {
            x->f_selectors[1] = 0;
            atom_setfloat(x->f_argv+1, 0.);
        }
        else if(argc > 1 && atom_gettype(argv+1) == A_FLOAT)
        {
            x->f_selectors[1] = 0;
            atom_setfloat(x->f_argv+1, atom_getfloat(argv+1));
        }
        else if(argc > 1 && atom_gettype(argv+1) == A_SYMBOL && (atom_getsymbol(argv+1) == gensym("s") || atom_getsymbol(argv+1) == gensym("symbol")))
        {
            x->f_selectors[1] = 1;
            atom_setsym(x->f_argv+1, gensym("symbol"));
        }
        else if(argc > 1 && atom_gettype(argv+1) == A_SYMBOL)
        {
            x->f_selectors[1] = 1;
            atom_setsym(x->f_argv+1, atom_getsymbol(argv+1));
        }
        else
        {
            x->f_selectors[1] = 0;
            atom_setfloat(x->f_argv+1, 0.);
        }
        for(i = 2; i < x->f_argc; i++)
        {
            eobj_proxynew(x);
            if(atom_gettype(argv+i) == A_SYMBOL && (atom_getsymbol(argv+i) == gensym("f") || atom_getsymbol(argv+i) == gensym("float")))
            {
                x->f_selectors[i] = 0;
                atom_setfloat(x->f_argv+i, 0.);
            }
            else if(atom_gettype(argv+i) == A_FLOAT)
            {
                x->f_selectors[i] = 0;
                atom_setfloat(x->f_argv+i, atom_getfloat(argv));
            }
            else if(atom_gettype(argv+i) == A_SYMBOL && (atom_getsymbol(argv+i) == gensym("s") || atom_getsymbol(argv+i) == gensym("symbol")))
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, gensym("symbol"));
            }
            else if(atom_gettype(argv+i) == A_SYMBOL)
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, atom_getsymbol(argv+i));
            }
            else
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, gensym("symbol"));
            }
        }
        x->f_out = outlet_new((t_object *)x, &s_list);
    }
    
    return (x);
}

extern "C" void setup_c0x2epak(void)
{
	t_eclass *c;
    
	c = eclass_new("c.pak", (method)pak_new, (method)pak_free, (short)sizeof(t_pak), CLASS_NOINLET, A_GIMME, 0);

    eclass_addmethod(c, (method)pak_anything,    "anything",       A_GIMME, 0);
    eclass_addmethod(c, (method)pak_list,        "list",           A_GIMME, 0);
    eclass_addmethod(c, (method)pak_float,       "float",          A_FLOAT, 0);
    eclass_addmethod(c, (method)pak_symbol,      "symbol",         A_SYMBOL,0);
    eclass_addmethod(c, (method)pak_output,      "bang",           A_NULL,  0);
    eclass_register(CLASS_OBJ, c);
    pak_class = c;
}

