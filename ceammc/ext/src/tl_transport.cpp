//
//  tl_transport.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 10/12/16.
//
//



#include "tl_lib.hpp"

t_class* tl_transport_class;

struct tl_transport_object {
    t_object x_obj;
};

static void tl_transport_cue(tl_transport_object* x,t_symbol *s, int argc, t_atom *argv)
{
    //printf("\n*cue\n");
    
    if (argc==1)
    {
        if (argv[0].a_type == A_FLOAT)
        {
            tll_perform_actions_for_cue(int(argv[0].a_w.w_float));
        }
    }
}

static void tl_transport_update(tl_transport_object* x,t_symbol *s, int argc, t_atom *argv)
{
    //printf("update\n");
    
    tll_cue_update_all_pos();
    tll_ui_update_all_pos();
    
}

static void tl_transport_info(tl_transport_object* x,t_symbol *s, int argc, t_atom *argv)
{
    //printf("info\n");
    
    tll_cue_dump();
    tll_ui_dump();
    
}

static void* tl_transport_new()//(t_symbol *s, int argc, t_atom *argv)
{
    tl_transport_object* x = reinterpret_cast<tl_transport_object*>(pd_new(tl_transport_class));
    
    //printf("transport-new\n");
    
    return static_cast<void*>(x);
}

extern "C" void setup_tl0x2etransport()
{
    tl_transport_class = class_new(gensym("tl.transport"),
                                  reinterpret_cast<t_newmethod>(tl_transport_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(tl_transport_object), 0, A_NULL);
   
    
    class_addmethod(tl_transport_class, reinterpret_cast<t_method>(tl_transport_info), gensym("info"), A_NULL);
    class_addmethod(tl_transport_class, reinterpret_cast<t_method>(tl_transport_update), gensym("update"), A_NULL);
    
    class_addmethod(tl_transport_class, reinterpret_cast<t_method>(tl_transport_cue), gensym("cue"), A_GIMME);
}
