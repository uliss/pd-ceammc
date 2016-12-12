#include "ceammc.h"
#include <m_pd.h>
#include <g_canvas.h>
#include <stdlib.h>
//

t_class* exp_patchdata_class;
struct t_exp_patchdata {
    t_object x_obj;
    
    t_canvas *c1;
    
    t_outlet *out1;
};

//extern t_pd *newest;
//
////copied
//static t_canvas *clone_makeone(t_symbol *s, int argc, t_atom *argv)
//{
//    t_canvas *retval;
//    newest = 0;
//    typedmess(&pd_objectmaker, s, argc, argv);
//    if (newest == 0)
//    {
//        error("clone: can't create subpatch '%s'",
//              s->s_name);
//        return (0);
//    }
//    if (*newest != canvas_class)
//    {
//        error("clone: can't clone '%s' because it's not an abstraction",
//              s->s_name);
//        pd_free(newest);
//        newest = 0;
//        return (0);
//    }
//    retval = (t_canvas *)newest;
//    newest = 0;
//    retval->gl_owner = 0;
//    retval->gl_isclone = 1;
//    return (retval);
//}

static void exp_patchdata_dump(t_exp_patchdata* x, t_symbol* s, int argc, t_atom *argv)
{
//    t_atom *noatom;
//    noatom->a_w.w_symbol = gensym("");
    
    //t_canvas *c1 = (t_canvas*)x->x_obj.te_g.g_next;
    //t_canvas *c1 = clone_makeone(gensym("pd new"), 0, noatom);
    
    //t_glist *glist=(t_glist *)canvas_getcurrent();
    //t_canvas *canvas=(t_canvas*)glist_getcanvas(glist);
    //
    //printf("canvas got\n");
    
    t_canvas * c1 = x->c1;
    
    if (c1){
        printf("canvas ptr: %lu\n", (long)c1);
        
        t_binbuf *b1 = binbuf_new();
        //t_canvas *canvas=(t_canvas*)glist_getcanvas(x->c1);
        
        printf("binbuf\n");
        canvas_saveto(x->c1, b1);
        printf("saveto\n");
        
//        t_gobj *y;
//        for (y = (x->c1->gl_list)?(x->c1->gl_list):0; y; y = y->g_next)
//        {
//            if (y->g_next)
//                gobj_save(y, b1);
//            printf("y ptr: %lu\n", (long)y);
//            
//        }
        
        printf("gettext");
        
        int blen=0;
        char *bchar;
        binbuf_gettext(b1, &bchar, &blen);
        t_atom *outlist = binbuf_getvec(b1);
        int outcount = binbuf_getnatom(b1);
        
        outlet_list(x->out1, &s_list, outcount, outlist);
    }
    
}

static void* exp_patchdata_new()
{
    //printf("new\n");
    
    t_exp_patchdata* x = reinterpret_cast<t_exp_patchdata*>(pd_new(exp_patchdata_class));
    
    x->c1 = canvas_getcurrent();
    
    x->out1 = outlet_new(&x->x_obj, &s_list);
    
    return static_cast<void*>(x);
    
    
}

extern "C" void setup_exp0x2epatchdata()
{
    //printf("setup\n");
    
    exp_patchdata_class = class_new(gensym("exp.patchdata"),
                                    reinterpret_cast<t_newmethod>(exp_patchdata_new),
                                    reinterpret_cast<t_method>(0),
                                    sizeof(t_exp_patchdata), 0, A_NULL);
    class_addmethod(exp_patchdata_class,reinterpret_cast<t_method>(exp_patchdata_dump), gensym("dump"), A_NULL);
    
}
