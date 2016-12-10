#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* list_integrator_class;
struct t_list_integrator {
    t_object x_obj;
    t_atom *stored_list;
    int stored_list_len;
};


static void list_integrator_clear(t_list_integrator* x, t_symbol* s, int argc, t_atom* argv)
{
    x->stored_list = (t_atom*)malloc(0);     //dummy
    x->stored_list_len = 0;
}

static void list_integrator_list(t_list_integrator* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;
    
    
    t_atom* temp_list =(t_atom*) malloc(sizeof(t_atom) * argc);
    for (int i=0;i<argc;i++)
    {
        temp_list[i] = argv[i];
    }
    
    int smallest_list_length = MIN(argc,x->stored_list_len);
    for (int i=0;i<smallest_list_length;i++)
    {
        argv[i].a_w.w_float += x->stored_list[i].a_w.w_float;
    }
    
    free(x->stored_list);
    
    x->stored_list = (t_atom*)malloc(argc * sizeof(t_atom));
    for (int i=0;i<argc;i++)
    {
        x->stored_list[i] = argv[i];
    }
    
    x->stored_list_len = argc;
    
    outlet_list(x->x_obj.te_outlet, s, argc, argv);
    
    free(temp_list);
    
}

static void* list_integrator_new()
{
    t_list_integrator* x = reinterpret_cast<t_list_integrator*>(pd_new(list_integrator_class));
    outlet_new(&x->x_obj, &s_list);
    
    x->stored_list = (t_atom*)malloc(0);     //dummy
    x->stored_list_len = 0;
    
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2eintegrator()
{
    list_integrator_class = class_new(gensym("list.integrator"),
                                  reinterpret_cast<t_newmethod>(list_integrator_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_list_integrator), 0, A_NULL);
    class_addlist(list_integrator_class, list_integrator_list);
    class_addanything(list_integrator_class, list_integrator_clear);
}
