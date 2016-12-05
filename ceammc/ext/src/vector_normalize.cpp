#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* vector_normalize_class;
struct t_vector_normalize {
    t_object x_obj;
    
};


static void vector_normalize_vector(t_vector_normalize* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 )
        return;
    
    float f_len = 0;
    
    for (int i=0;i<argc;i++)
    {
        f_len += argv[i].a_w.w_float * argv[i].a_w.w_float;
    }
    
    f_len = qsqrt(f_len);
    
    for (int i=0;i<argc;i++)
    {
        argv[i].a_w.w_float /= f_len;
    }
    
    outlet_list(x->x_obj.te_outlet, s, argc, argv);
    
}

static void* vector_normalize_new()
{
    t_vector_normalize* x = reinterpret_cast<t_vector_normalize*>(pd_new(vector_normalize_class));
    outlet_new(&x->x_obj, &s_list);
    
    
    return static_cast<void*>(x);
}

extern "C" void setup_vector0x2enormalize()
{
    vector_normalize_class = class_new(gensym("vector.normalize"),
                                  reinterpret_cast<t_newmethod>(vector_normalize_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_vector_normalize), 0, A_NULL);
    class_addlist(vector_normalize_class, vector_normalize_vector);
    
    
    
}
