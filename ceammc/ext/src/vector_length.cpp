#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* vector_length_class;
struct t_vector_length {
    t_object x_obj;
    
};


static void vector_length_list(t_vector_length* x, t_symbol* s, int argc, t_atom* argv)
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

static void* vector_length_new()
{
    t_vector_length* x = reinterpret_cast<t_vector_length*>(pd_new(vector_length_class));
    outlet_new(&x->x_obj, &s_float);
    
    
    return static_cast<void*>(x);
}

extern "C" void setup_vector0x2elength()
{
    vector_length_class = class_new(gensym("vector.length"),
                                  reinterpret_cast<t_newmethod>(vector_length_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_vector_length), 0, A_NULL);
    class_addlist(vector_length_class, vector_length_list);
    
    
    
}
