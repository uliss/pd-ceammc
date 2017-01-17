#include "oop_common.h"

#define OBJ_NAME "exp.method"

t_eclass* exp_method_class;


//////////////



//////////////

struct t_exp_method {
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    
    t_symbol *method_name;
    
    oPDInstance *instance;
    
    t_outlet *out1;
};

static void *exp_method_new(t_symbol *id, int argc, t_atom *argv)
    {
        
        if (argc<1)
        {
            error("missing method name argument");
            return 0;
        }
        
        t_exp_method* x = reinterpret_cast<t_exp_method*>(eobj_new(exp_method_class));
        x->parent_canvas = canvas_getcurrent();
        
        t_binbuf* d = binbuf_via_atoms(argc,argv);
        
        if (x && d)
        {
            
            ebox_attrprocess_viabinbuf(x, d);
            
            
        }
        
        ceammc::Atom a = (argv[0]);
        x->method_name = a.asSymbol();
        
        std::string str = to_string(x->parent_canvas);
        x->instance = new oPDInstance(str, OBJ_NAME);
        
        ebox_ready((t_ebox *)x);
        
        x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
        x->e_box.b_boxparameters.d_bordercolor = rgba_green;
        
        x->out1 = outlet_new((t_object*)x, &s_anything);
        
        x->instance->ref().addMethod(x->method_name, x->out1);
        
        return static_cast<void*>(x);

}


static void exp_method_free(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
    x->instance->ref().freeMethod(x->method_name);
    
}

static void exp_method_any(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
      if (x->instance)
      {
          ceammc::AtomList list(argc,argv);
          x->instance->ref().multipleOutput(list);

      }
}

extern "C" void setup_exp0x2emethod()
{
    
    exp_method_class = eclass_new((OBJ_NAME),
                                    reinterpret_cast<t_typ_method>(exp_method_new),
                                    reinterpret_cast<t_typ_method>(exp_method_free),
                                    sizeof(t_exp_method), CLASS_PATCHABLE, A_GIMME,0);
    
    //eclass_guiinit(exp_method_class, 0);
    
    eclass_addmethod(exp_method_class, (method)(exp_method_any), ("anything"), A_GIMME,0);
    
    
    eclass_register(CLASS_BOX, exp_method_class);

}