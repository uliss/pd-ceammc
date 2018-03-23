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

typedef struct  _patcherargs
{
	t_eobj      j_box;
	t_outlet*   f_out_args;
    t_outlet*   f_out_attrs;
    t_outlet*   f_out_done;
    t_canvas*   f_cnv;
    t_atom*     f_args;
    int         f_argc;

    int         f_nattrs;
    t_symbol**  f_attr_name;
    t_atom**    f_attr_vals;
    int*        f_attr_size;
    char        f_init;

} t_patcherargs;

t_eclass *patcherargs_class;

static char patcherargs_initialize(t_patcherargs *x)
{
    t_canvas* cnv = x->f_cnv;
    if(cnv)
    {
        t_binbuf *b = cnv->gl_obj.te_binbuf;
        if(b)
        {
            int ac      = binbuf_getnatom(b);
            t_atom* av  = binbuf_getvec(b);
            if(atom_gettype(av) == A_SYMBOL && atom_getsymbol(av) == gensym("pd"))
            {
                ac--;
                av++;
            }
            int argc = atoms_get_attributes_offset(ac, av);
            
            if(argc > x->f_argc)
            {
                x->f_argc = argc;
                x->f_args = (t_atom *)realloc(x->f_args, (size_t)x->f_argc * sizeof(t_atom));
                if(!x->f_args)
                {
                    x->f_argc = 0;
                    pd_error(x, "can't allocate memory");
                    return 0;
                }
            }
            
            memcpy(x->f_args, av, (size_t)argc * sizeof(t_atom));
            
            for(int i = 0; i < x->f_nattrs; i++)
            {
                int nattr;
                t_atom* attrs;
                if(!atoms_get_attribute((int)(ac-argc), av+argc, x->f_attr_name[i], &nattr, &attrs))
                {
                    x->f_attr_size[i] = nattr;
                    x->f_attr_vals[i] = (t_atom *)realloc(x->f_attr_vals[i], (size_t)nattr * sizeof(t_atom));
                    memcpy(x->f_attr_vals[i], attrs, (size_t)nattr * sizeof(t_atom));
                    free(attrs);
                }
            }
            return 1;
        }
    }
    return 0;
}

static void patcherargs_output(t_patcherargs *x)
{
    if(!x->f_init)
    {
        x->f_init = patcherargs_initialize(x);
    }
    outlet_list(x->f_out_args, &s_list, (int)x->f_argc, x->f_args);
    for(int i = 0; i < x->f_nattrs; i++)
    {
        outlet_anything(x->f_out_attrs, gensym(x->f_attr_name[i]->s_name+1), x->f_attr_size[i], x->f_attr_vals[i]);
    }
    outlet_bang(x->f_out_done);
}

static void patcherargs_free(t_patcherargs *x)
{
    int i;
    if(x->f_argc && x->f_args)
    {
        free(x->f_args);
    }
    if(x->f_nattrs)
    {
        for(i = 0; i < x->f_nattrs; i++)
        {
            if(x->f_attr_size[i] && x->f_attr_vals[i])
            {
                free(x->f_attr_vals[i]);
            }
        }
        free(x->f_attr_name);
        free(x->f_attr_size);
        free(x->f_attr_vals);
    }
    eobj_free(x);
}

static void *patcherargs_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
    t_patcherargs *x = (t_patcherargs *)eobj_new(patcherargs_class);
    if(x)
    {
        x->f_argc = atoms_get_attributes_offset(argc, argv);
        x->f_args = (t_atom *)malloc((size_t)x->f_argc * sizeof(t_atom));
        memcpy(x->f_args, argv, (size_t)x->f_argc * sizeof(t_atom));
        
        x->f_nattrs = atoms_get_keys((int)(argc-x->f_argc), argv+x->f_argc, &x->f_attr_name);
        if(x->f_nattrs)
        {
            x->f_attr_vals = (t_atom **)malloc((size_t)x->f_nattrs * sizeof(t_atom *));
            x->f_attr_size = (int *)malloc((size_t)x->f_nattrs * sizeof(int));
            for(i = 0; i < x->f_nattrs; i++)
            {
                atoms_get_attribute(argc-x->f_argc, argv+x->f_argc, x->f_attr_name[i], &x->f_attr_size[i], &x->f_attr_vals[i]);
            }
        }

        x->f_out_args = outlet_new((t_object *)x, &s_list);
        x->f_out_attrs = outlet_new((t_object *)x, &s_list);
        x->f_out_done = outlet_new((t_object *)x, &s_bang);
        if(canvas_getcurrent())
        {
            x->f_cnv = glist_getcanvas(canvas_getcurrent());
        }
        else
            x->f_cnv = NULL;
        
        x->f_init = patcherargs_initialize(x);
    }
    
    return (x);
}


extern "C" void setup_c0x2epatcherargs(void)
{
	t_eclass *c;

	c = eclass_new("c.patcherargs", (method)patcherargs_new, (method)patcherargs_free, (short)sizeof(t_patcherargs), 0L, A_GIMME, 0);
    class_addcreator((t_newmethod)patcherargs_new, gensym("c.canvasargs"), A_GIMME, 0);
    eclass_addmethod(c, (method)patcherargs_output,      "bang",       A_NULL, 0);
    eclass_register(CLASS_OBJ, c);
    patcherargs_class = c;
}


