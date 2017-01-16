#include "ceammc.h"
#include <m_pd.h>
//#include <g_canvas.h>
#include <stdlib.h>
//
#include "ceammc_atomlist.h"

#include "m_imp.h"

#include <stdio.h>

#include "cicm_wrapper.h"

#include "ceammc_globaldata.h"

t_eclass* exp_class_class;
typedef ceammc::GlobalData<t_canvas*> oPDClass;

struct t_exp_class {
    //t_object x_obj;
    
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    t_canvas *sub_canvas;
    
    oPDClass *global;
    
//    t_binbuf *patch;
//    
//    t_canvas *new_canvas;
//    
//    t_inlet* inlets[64];
//    int inlet_count;
//    t_outlet* outlets[64];
//    int outlet_count;
    
};

#define OBJ_NAME "exp.class"

using namespace ceammc;

//inline int get_num_inlets(AtomList input)
//{
//    int ret =0;
//    for (int i=0;i<input.size();i++)    //iterator
//    {
//        if (input.at(i)==gensym("inlet"))
//            ret++;
//    }
//    
//    return ret>64?64:ret;
//}
//
//inline int get_num_outlets(AtomList input)
//{
//    int ret =0;
//    for (int i=0;i<input.size();i++)    //iterator
//    {
//        if (input.at(i)==gensym("outlet"))
//            ret++;
//    }
//    
//    return ret>64?64:ret;
//}
//
//inline int process_class_object(AtomList input)
//{
//    int ret = 1;
//    for (int i=0;i<input.size();i++)    //iterator
//    {
//        if (input.at(i)==gensym("exp.class"))
//        {
//            Atom *r_c = &input.at(i+1);
//            r_c->setFloat( r_c->asInt() - 1 );
//            if (r_c<=0)
//            {
//                ret = 0;  //disable class
//            }
//        }
//    }
//    
//    return ret;
//}
//
//
//static void exp_class_inlet_proxy(t_exp_class* x, t_symbol* s, int argc, t_atom *argv)
//{
//    if (x->new_canvas) {
//        
//        if (argc <= 0)
//            return;
////        else if (argv->a_type == A_SYMBOL)
////            
////            obj_sendinlet(&x->e_box.b_obj.o_obj.te_  gl_obj, x->i_n,
////                          argv[0].a_w.w_symbol, argc-1, argv+1);
////        else obj_sendinlet(&x->i_owner->x_vec[phase].c_gl->gl_obj, x->i_n,
////                           &s_list, argc, argv);
//        
//        
////        obj_list(&x->e_box.b_obj.o_obj, s, argc, argv); //&x->new_canvas->gl_obj.ob_pd
//        
//        
//    }
//}
//
////*****
//
//static int canvas_scanbinbuf(int natoms, t_atom *vec, int *p_indexout,
//                             int *p_next)
//{
//    int i, j;
//    int indexwas = *p_next;
//    *p_indexout = indexwas;
//    if (indexwas >= natoms)
//        return (0);
//    for (i = indexwas; i < natoms && vec[i].a_type != A_SEMI; i++)
//        ;
//    if (i >= natoms)
//        *p_next = i;
//    else *p_next = i + 1;
//    return (i - indexwas);
//}
//
//static void canvas_readerror(int natoms, t_atom *vec, int message,
//                             int nline, char *s)
//{
//    error("%s", s);
//    startpost("line was:");
//    postatom(nline, vec + message);
//    endpost();
//}
//
//void glist_readfrombinbuf(t_glist *x, t_binbuf *b, char *filename, int selectem)
//{
//    t_canvas *canvas = glist_getcanvas(x);
//    int cr = 0, natoms, nline, message, nextmsg = 0, i, j, nitems;
//    t_atom *vec;
//    t_gobj *gobj;
//    
//    natoms = binbuf_getnatom(b);
//    vec = binbuf_getvec(b);
//    
//    
//    /* check for file type */
//    nline = canvas_scanbinbuf(natoms, vec, &message, &nextmsg);
//    if (nline != 1 && vec[message].a_type != A_SYMBOL &&
//        strcmp(vec[message].a_w.w_symbol->s_name, "data"))
//    {
//        pd_error(x, "%s: file apparently of wrong type", filename);
//        return;
//    }
//    /* read in templates and check for consistency */
//    while (1)
//    {
//        t_template *newtemplate, *existtemplate;
//        t_symbol *templatesym;
//        t_atom *templateargs = (t_atom*)getbytes(0);
//        int ntemplateargs = 0, newnargs;
//        nline = canvas_scanbinbuf(natoms, vec, &message, &nextmsg);
//        if (nline < 2)
//        {
//            t_freebytes(templateargs, sizeof (*templateargs) * ntemplateargs);
//            break;
//        }
//        else if (nline > 2)
//            canvas_readerror(natoms, vec, message, nline,
//                             "extra items ignored");
//        else if (vec[message].a_type != A_SYMBOL ||
//                 strcmp(vec[message].a_w.w_symbol->s_name, "template") ||
//                 vec[message + 1].a_type != A_SYMBOL)
//        {
//            canvas_readerror(natoms, vec, message, nline,
//                             "bad template header");
//            continue;
//        }
//        templatesym = canvas_makebindsym(vec[message + 1].a_w.w_symbol);
//        while (1)
//        {
//            nline = canvas_scanbinbuf(natoms, vec, &message, &nextmsg);
//            if (nline != 2 && nline != 3)
//                break;
//            newnargs = ntemplateargs + nline;
//            templateargs = (t_atom *)t_resizebytes(templateargs,
//                                                   sizeof(*templateargs) * ntemplateargs,
//                                                   sizeof(*templateargs) * newnargs);
//            templateargs[ntemplateargs] = vec[message];
//            templateargs[ntemplateargs + 1] = vec[message + 1];
//            if (nline == 3)
//                templateargs[ntemplateargs + 2] = vec[message + 2];
//            ntemplateargs = newnargs;
//        }
//        if (!(existtemplate = template_findbyname(templatesym)))
//        {
//            error("%s: template not found in current patch",
//                  templatesym->s_name);
//            t_freebytes(templateargs, sizeof (*templateargs) * ntemplateargs);
//            return;
//        }
//        newtemplate = template_new(templatesym, ntemplateargs, templateargs);
//        t_freebytes(templateargs, sizeof (*templateargs) * ntemplateargs);
//        if (!template_match(existtemplate, newtemplate))
//        {
//            error("%s: template doesn't match current one",
//                  templatesym->s_name);
//            pd_free(&newtemplate->t_pdobj);
//            return;
//        }
//        pd_free(&newtemplate->t_pdobj);
//    }
//    while (nextmsg < natoms)
//    {
//        canvas_readscalar(x, natoms, vec, &nextmsg, selectem);
//    }
//}
//
////*****
//
//static void exp_class_update(t_exp_class* x, t_symbol* s, int argc, t_atom *argv)
//{
//    //todo error check
//    
////    if (x->patch)
////    {
////        binbuf_free(x->patch);
////    }
////    x->patch = binbuf_new();
////    
////    canvas_saveto(x->parent_canvas, x->patch);
////    
////    int blen=0;
////    char *bchar;
////    binbuf_gettext(x->patch, &bchar, &blen);
////    t_atom *outlist = binbuf_getvec(x->patch);
////    int outcount = binbuf_getnatom(x->patch);
////    
////    AtomList list_ = AtomList(outcount, outlist);
////    
////    if (process_class_object(list_))
////    {
////        x->inlet_count = get_num_inlets(list_);
////        x->outlet_count = get_num_outlets(list_);
////        
////        if (x->new_canvas) canvas_free(x->new_canvas);
////        
////        x->new_canvas = canvas_new(0, gensym("new_canvas"), 0, NULL);
////        x->new_canvas->gl_owner = 0;
////        x->new_canvas->gl_isclone = 1;
////        
////        for (int i=0;i<(x->inlet_count-1);i++)
////        {
////            x->inlets[i] = inlet_new(&x->e_box.b_obj.o_obj, &x->e_box.b_obj.o_obj.ob_pd, &s_anything, &s_anything);
////            
////            canvas_addinlet(x->new_canvas, &x->e_box.b_obj.o_obj.ob_pd, &s_anything);
////        }
////        
////        for (int i=0;i<x->outlet_count;i++)
////        {
////            x->outlets[i] = outlet_new(&x->e_box.b_obj.o_obj, &s_anything);
////            
////            canvas_addoutlet(x->new_canvas, &x->e_box.b_obj.o_obj.ob_pd, &s_anything);
////            
////        }
////        
////        glist_readfrombinbuf(x->new_canvas, x->patch, "", 0);
//    
////        for (int i=0;i<list_.size();i++)    //iterator
////        {
////            
////            
////        }
//        
//        
//        
//        
//    }
//    
//}





static void* exp_class_new(t_symbol *id, int argc, t_atom *argv)
{
    
    
    if (argc<1)
    {
        error("no class name provided!"); return 0;
    }
    
    Atom a = argv[0];
    if (!a.isSymbol())
    {
        error("bad class name!"); return 0;
    }
    
    t_exp_class* x = reinterpret_cast<t_exp_class*>(eobj_new(exp_class_class));
    
    x->parent_canvas = canvas_getcurrent();
    
    
    
    
//    if (x->canvas && x->canvas->gl_obj.ob_binbuf)
//    {
//        exp_class_update(x, NULL, 0, NULL);
//    }
    
    
    
    //x->sub_canvas->gl_owner = x->e_box.b_obj.o_canvas;
    
    //canvas_vis(x->sub_canvas, 1);
    
    //should be read only for others
    x->global = new oPDClass(a.asString(), OBJ_NAME);
    
    if (!x->global->ref())
    {
        x->sub_canvas = (t_canvas*)subcanvas_new(gensym(a.asString().c_str())); //LISP lol
        x->sub_canvas->gl_havewindow = 1;
        //x->sub_canvas->gl_env = 0;
        x->sub_canvas->gl_isclone = 1;
        
        x->global->ref() = x->sub_canvas;
        
        post("class construct");
    }
    else
    {
        x->sub_canvas = x->global->ref();
    }
    
    return static_cast<void*>(x);
    
    
}

static void exp_class_vis(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc<1) return;
    Atom a = argv[0];
    
    if (!a.isFloat()) return;
        
    if (x->sub_canvas)
    {
        post("vis");
        canvas_vis(x->sub_canvas, (a.asInt()>0));
    }
}


static void exp_class_free(t_exp_class* x)
{
    canvas_free(x->sub_canvas);
    
    delete x->global;
}

extern "C" void setup_exp0x2eclass()
{
    
    exp_class_class = eclass_new((OBJ_NAME),
                                    reinterpret_cast<t_typ_method>(exp_class_new),
                                    reinterpret_cast<t_typ_method>(0),
                                    sizeof(t_exp_class), 0, A_GIMME,0);
    
    eclass_addmethod(exp_class_class,(t_typ_method)(exp_class_vis), ("vis"), A_GIMME,0);
    
//    eclass_addmethod(exp_class_class,(t_typ_method)(exp_class_update), ("update"), A_NULL,0);
//    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_inlet_proxy), ("anything"), A_GIMME, 0);
}
