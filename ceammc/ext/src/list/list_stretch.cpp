#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* list_stretch_class;
struct t_list_stretch {
    t_object x_obj;
    t_float new_len;
    
    t_atom *output;

};


static void list_stretch_list(t_list_stretch* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 || x->new_len<1)
        return;
    
    if (x->output) free(x->output);
    x->output = (t_atom*)malloc(sizeof(t_atom)*x->new_len);
    
    for (int i=0;i<x->new_len;i++)
    {
        float new_index = (float(i)/x->new_len)*argc;
        
        int idx1 = int(new_index);
        int idx2 = ((idx1+1)<argc)? (idx1+1) : (idx1);
        float mix1 = new_index - idx1;
        
        x->output[i].a_type = A_FLOAT;      //?
        x->output[i].a_w.w_float = argv[idx1].a_w.w_float * (1-mix1) + argv[idx2].a_w.w_float * mix1;
        
    }
    
    outlet_list(x->x_obj.te_outlet, s, x->new_len, x->output);
    
}

static void* list_stretch_new()
{
    t_list_stretch* x = reinterpret_cast<t_list_stretch*>(pd_new(list_stretch_class));
    outlet_new(&x->x_obj, &s_list);
    
    x->output = (t_atom*)malloc(0);         //dummy
    
    floatinlet_new(&x->x_obj, &x->new_len);
    
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2estretch()
{
    list_stretch_class = class_new(gensym("list.stretch"),
                                  reinterpret_cast<t_newmethod>(list_stretch_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_list_stretch), 0, A_NULL);
    class_addlist(list_stretch_class, list_stretch_list);
    
    
    
}
