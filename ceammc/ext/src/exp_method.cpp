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

#define OBJ_NAME "exp.method"

t_eclass* exp_method_class;


//////////////

class t_instance
{
public:
    std::string class_name;
    t_canvas *canvas;
    t_outlet *inst_out;
    
} ;
typedef ceammc::GlobalData<t_instance> oPDInstance;


#include <sstream>

template <class T>
inline std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

//////////////

struct t_exp_method {
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    
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
        
        std::string str = to_string(x->parent_canvas);
        x->instance = new oPDInstance(str, OBJ_NAME);
        
        ebox_ready((t_ebox *)x);
        
        x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
        x->e_box.b_boxparameters.d_bordercolor = rgba_green;
        
        x->out1 = outlet_new((t_object*)x, &s_anything);
        
        //register method
        
        return static_cast<void*>(x);

}

static void exp_method_any(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
      if (x->instance)
      {
          if (x->instance->ref().inst_out)
          {
              ceammc::AtomList list(argc,argv);
            
              list.output(x->instance->ref().inst_out);
          }
      }
}

extern "C" void setup_exp0x2emethod()
{
    
    exp_method_class = eclass_new((OBJ_NAME),
                                    reinterpret_cast<t_typ_method>(exp_method_new),
                                    reinterpret_cast<t_typ_method>(0),
                                    sizeof(t_exp_method), CLASS_PATCHABLE, A_GIMME,0);
    
    //eclass_guiinit(exp_method_class, 0);
    
    eclass_addmethod(exp_method_class, (method)(exp_method_any), ("anything"), A_GIMME,0);
    
    
    eclass_register(CLASS_BOX, exp_method_class);

}