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

typedef struct  _patchermess
{
	t_eobj      j_box;
    t_canvas*   f_canvas;
    t_symbol*   f_s;
    int         f_argc;
    t_atom*     f_argv;
} t_patchermess;

t_eclass *patchermess_class;

static void doselect(t_canvas *canvas, int pos_x, int pos_y, int pos_w, int pos_h)
{
    t_gobj *y;
    for(y = canvas->gl_list; y; y = y->g_next)
    {
        if(!glist_isselected(canvas, y))
        {
            int npos_x, npos_y, npos_w, npos_h;
            gobj_getrect(y, canvas, &npos_x, &npos_y, &npos_w, &npos_h);
            npos_w -= npos_x;
            npos_h -= npos_y;
            if(npos_x + npos_w > pos_x && npos_y + npos_h > pos_y && npos_x < pos_x + pos_w && npos_y < pos_y + pos_h)
            {
                glist_select(canvas, y);
            }
        }
    }
}

static void patchermess_anything(t_patchermess *x, t_symbol *s, int argc, t_atom *argv)
{
    t_canvas *canvas = eobj_getcanvas(x);
    if(argc && argv && canvas)
    {
        if(s == cream_sym_select && canvas->gl_editor && argc > 3 && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT && atom_gettype(argv+3) == A_FLOAT)
        {
            doselect(canvas, atom_getfloat(argv), atom_getfloat(argv+1), atom_getfloat(argv+2) - atom_getfloat(argv), atom_getfloat(argv+3) - atom_getfloat(argv+1));
        }
        else if(s == cream_sym_delete && canvas->gl_editor && argc > 3 && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT && atom_gettype(argv+3) == A_FLOAT)
        {
            t_gobj *y;
            t_atom av[3];
            unsigned int edit = canvas->gl_edit;
            unsigned int dirty = canvas->gl_dirty;
            atom_setfloat(av, 1);
            pd_typedmess((t_pd *)canvas, gensym("editmode"), 1, av);
            int pos_x = atom_getfloat(argv), pos_y = atom_getfloat(argv+1), pos_w = atom_getfloat(argv+2) - atom_getfloat(argv), pos_h = atom_getfloat(argv+3) - atom_getfloat(argv+1);
            for(y = canvas->gl_list; y; y = y->g_next)
            {
                if(!glist_isselected(canvas, y))
                {
                    int npos_x, npos_y, npos_w, npos_h;
                    gobj_getrect(y, canvas, &npos_x, &npos_y, &npos_w, &npos_h);
                    npos_w -= npos_x;
                    npos_h -= npos_y;
                    if(npos_x + npos_w > pos_x && npos_y + npos_h > pos_y && npos_x < pos_x + pos_w && npos_y < pos_y + pos_h)
                    {
                        glist_select(canvas, y);
                    }
                }
            }
            atom_setfloat(av, 1);
            atom_setfloat(av+1, 127);
            atom_setfloat(av+2, 0);
            pd_typedmess((t_pd *)canvas, gensym("key"), 3, av);
            atom_setfloat(av, edit);
            pd_typedmess((t_pd *)canvas, gensym("editmode"), 1, av);
            if(!dirty && argc > 4 && atom_gettype(argv+4) == A_FLOAT && atom_getfloat(argv+4) == 0)
            {
                canvas->gl_dirty = 0;
            }
        }
        else if(s == cream_sym_past && canvas->gl_editor && atom_gettype(argv) == A_SYMBOL)
        {
            unsigned int edit = canvas->gl_edit;
            unsigned int dirty = canvas->gl_dirty;
            t_canvas* newcnv = NULL;
            char dirbuf[MAXPDSTRING], *nameptr;
            int fd = canvas_open(canvas, atom_getsymbol(argv)->s_name, ".pd", dirbuf, &nameptr, MAXPDSTRING, 0);
            if(fd >= 0)
            {
                int dsp = canvas_suspend_dsp();
                t_pd *boundx = s__X.s_thing;
                s__X.s_thing = 0;
                binbuf_evalfile(gensym(nameptr), gensym(dirbuf));
                while(((t_pd *)newcnv != s__X.s_thing) && s__X.s_thing)
                {
                    newcnv = (t_canvas *)s__X.s_thing;
                    vmess((t_pd *)newcnv, gensym("pop"), (char *)"i", 1);
                    
                    pd_typedmess((t_pd *)newcnv, gensym("selectall"), 0, NULL);
                    pd_typedmess((t_pd *)newcnv, gensym("copy"), 0, NULL);
                    pd_typedmess((t_pd *)canvas, cream_sym_past, 0, NULL);
                    canvas_free(newcnv);
                    t_atom av;
                    atom_setfloat(&av, edit);
                    pd_typedmess((t_pd *)canvas, gensym("editmode"), 1, &av);
                    if(!dirty && argc > 1 && atom_gettype(argv+1) == A_FLOAT && atom_getfloat(argv+1) == 0)
                    {
                        canvas->gl_dirty = 0;
                    }
                }
                s__X.s_thing = boundx;
                canvas_resume_dsp(dsp);
            }
            
        }
        else if(s == cream_sym_goppos && (canvas->gl_isgraph || canvas->gl_goprect))
        {
            t_atom av[9];
            atom_setfloat(av, canvas->gl_x1);
            atom_setfloat(av+1, canvas->gl_y1);
            atom_setfloat(av+2, canvas->gl_x2);
            atom_setfloat(av+3, canvas->gl_y2);
            atom_setfloat(av+4, canvas->gl_pixwidth);
            atom_setfloat(av+5, canvas->gl_pixheight);
            atom_setfloat(av+6, 1.f);
            if(argc && atom_gettype(argv) == A_FLOAT)
                atom_setfloat(av+7, atom_getfloat(argv));
            if(argc > 1 && atom_gettype(argv+1) == A_FLOAT)
                atom_setfloat(av+8, atom_getfloat(argv+1));
            pd_typedmess((t_pd *)canvas, gensym("coords"), 9, av);
        }
        else if(s == cream_sym_gopsize && (canvas->gl_isgraph || canvas->gl_goprect))
        {
            t_atom av[9];
            atom_setfloat(av, canvas->gl_x1);
            atom_setfloat(av+1, canvas->gl_y1);
            atom_setfloat(av+2, canvas->gl_x2);
            atom_setfloat(av+3, canvas->gl_y2);
            if(argc && atom_gettype(argv) == A_FLOAT)
                atom_setfloat(av+4, atom_getfloat(argv));
            if(argc > 1 && atom_gettype(argv+1) == A_FLOAT)
                atom_setfloat(av+5, atom_getfloat(argv+1));
            atom_setfloat(av+6, 1.f);
            atom_setfloat(av+7, canvas->gl_xmargin);
            atom_setfloat(av+8, canvas->gl_ymargin);
            pd_typedmess((t_pd *)eobj_getcanvas(x), gensym("coords"), 9, av);
        }
        else
        {
            pd_typedmess((t_pd *)eobj_getcanvas(x), s, argc, argv);
        }
    }
}

static void *patchermess_new(t_symbol *s, int argc, t_atom *argv)
{
    t_patchermess *x = (t_patchermess *)eobj_new(patchermess_class);
    if(x)
    {
        if(canvas_getcurrent())
        {
            x->f_canvas = glist_getcanvas(canvas_getcurrent());
        }
        else
        {
            x->f_canvas = NULL;
        }
    }
    return (x);
}

extern "C" void setup_c0x2epatchermess(void)
{
	t_eclass *c;
    
	c = eclass_new("c.patchermess", (method)patchermess_new, (method)eobj_free, (short)sizeof(t_patchermess), 0L, A_GIMME, 0);
    class_addcreator((t_newmethod)patchermess_new, gensym("c.canvasmess"), A_GIMME, 0);

    eclass_addmethod(c, (method)patchermess_anything, "anything", A_GIMME, 0);
    eclass_register(CLASS_OBJ, c);
    patchermess_class = c;
}



