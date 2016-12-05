#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* list_vlength_class;
struct t_list_vlength {
    t_object x_obj;
    
};


static void list_vlength_list(t_list_vlength* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 )
        return;
    
    float f_out = 0;
    
    for (int i=0;i<argc;i++)
    {
        f_out += argv[i].a_w.w_float * argv[i].a_w.w_float;
    }
    
    f_out = qsqrt(f_out);
    
    outlet_float(x->x_obj.te_outlet, f_out);
    
}

static void* list_vlength_new()
{
    t_list_vlength* x = reinterpret_cast<t_list_vlength*>(pd_new(list_vlength_class));
    outlet_new(&x->x_obj, &s_float);
    
    
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2evlength()
{
    list_vlength_class = class_new(gensym("list.vlength"),
                                  reinterpret_cast<t_newmethod>(list_vlength_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_list_vlength), 0, A_NULL);
    class_addlist(list_vlength_class, list_vlength_list);
    
    
    
}
