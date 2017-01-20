//
//  exp_aproperty.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 20/01/17.
//
//

#include "oop_common.h"

#define OBJ_NAME "exp.aproperty"

t_eclass* exp_method_class;

typedef ceammc::GlobalData<ceammc::AtomList> LocalList;

using namespace ceammc;

struct t_exp_method {
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    
    t_symbol *property_name;
    
    oPDInstance *instance;
    
    LocalList *x_global;
    
    t_canvas *canvas;
    
    t_outlet *out1, *out2, *out3;
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
        
        
        x->canvas = canvas_getcurrent();
        
        //TODO fix
        char buf[16];
        sprintf(buf, "%lu", (long)x->canvas);
        
        std::string name = id->s_name;
        name += buf;
        
        x->x_global = new LocalList(name.c_str(), OBJ_NAME);
        
        Atom a = (argv[0]);
        x->property_name = a.asSymbol();
        
        std::string str = to_string(x->parent_canvas);
        x->instance = new oPDInstance(str, OBJ_NAME);
        
        x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
        x->e_box.b_boxparameters.d_bordercolor = rgba_green;
        
        x->out1 = outlet_new((t_object*)x, &s_anything);
        x->out2 = outlet_new((t_object*)x, &s_anything);    //setter bang
        x->out3 = outlet_new((t_object*)x, &s_anything);    //getter bang
        
        //std::string getter_name = "get_" + std::string(x->property_name->s_name);
        //std::string setter_name = "set_" + std::string(x->property_name->s_name);
        
        //x->instance->ref().addMethod(gensym(setter_name.c_str()), x->out1);
        //x->instance->ref().addMethod(gensym(getter_name.c_str()), x->out2);
        
        x->instance->ref().addPropertyBox(x->property_name, (t_object*)x);
        
        ebox_ready((t_ebox *)x);
        
        
        return static_cast<void*>(x);

}


static void exp_method_free(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
    x->instance->ref().freeMethod(x->property_name);
    
}

#pragma mark -

static void exp_method_set(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
    //AtomList list((Atom(id)));
    
    //poststring(id->s_name);
    
    Atom name = Atom(argv[0]);
    
    if (name.asSymbol() == x->property_name)
    {
        //AtomList list2(argc,argv);
        //list.append(list2);
        
        x->x_global->ref().fromPdData(static_cast<size_t>(argc-1), &argv[1]);
        
        outlet_bang(x->out2);
    }
}


static void exp_method_output(t_exp_method* x)
{
    ceammc::to_outlet(x->out1, x->x_global->ref());
}

static void exp_method_bang(t_exp_method* x)
{
    exp_method_output(x);
}

//rename
static void exp_method_list(t_exp_method* x, t_symbol* s, int argc, t_atom* argv)
{
    x->x_global->ref().fromPdData(static_cast<size_t>(argc), argv);
    exp_method_output(x);
}


static void exp_method_get(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
    
    //poststring(id->s_name);
    
    if (x->instance)
    {
        AtomList list((Atom(argv[0])));
        list.append(x->x_global->ref());
        x->instance->ref().multipleOutput(list);
        
    }
    
    outlet_bang(x->out3);
    
}

//static void exp_method_any(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
//{
//      if (x->instance)
//      {
//          AtomList list((Atom(id)));
//          AtomList list2(argc,argv);
//          list.append(list2);
//          x->instance->ref().multipleOutput(list);
//
//      }
//}

extern "C" void setup_exp0x2eaproperty()
{
    
    exp_method_class = eclass_new((OBJ_NAME),
                                    reinterpret_cast<t_typ_method>(exp_method_new),
                                    reinterpret_cast<t_typ_method>(exp_method_free),
                                    sizeof(t_exp_method), CLASS_PATCHABLE, A_GIMME,0);
    
    //eclass_guiinit(exp_method_class, 0);
    
    //eclass_addmethod(exp_method_class, (method)(exp_method_any), ("anything"), A_GIMME,0);
    
    eclass_addmethod(exp_method_class, (method)(exp_method_set), ("set"), A_GIMME, 0);
    eclass_addmethod(exp_method_class, (method)(exp_method_get), ("get"), A_GIMME, 0);
    
    eclass_addmethod(exp_method_class, (method)(exp_method_bang), ("bang"), A_GIMME, 0);
    eclass_addmethod(exp_method_class, (method)(exp_method_list), ("anything"), A_GIMME, 0);
    
    
    eclass_register(CLASS_BOX, exp_method_class);

}