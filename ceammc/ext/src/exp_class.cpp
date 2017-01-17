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
    
    t_outlet *out1;
    
};

#define OBJ_NAME "exp.class"

using namespace ceammc;




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
    
    
    //should be read only for others
    x->global = new oPDClass(a.asString(), OBJ_NAME);
    
    if (!x->global->ref())
    {
        x->sub_canvas = (t_canvas*)subcanvas_new(gensym(a.asString().c_str())); //LISP lol
        x->sub_canvas->gl_havewindow = 1;
        //x->sub_canvas->gl_env = 0;
        x->sub_canvas->gl_isclone = 1;
        
        x->global->ref() = x->sub_canvas;
        
        canvas_vis(x->sub_canvas, 0);

    }
    else
    {
        x->sub_canvas = x->global->ref();
    }
    
    x->out1 = outlet_new((t_object*)x, &s_anything);
    
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
