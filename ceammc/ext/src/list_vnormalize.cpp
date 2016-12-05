#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* list_vnormalize_class;
struct t_list_vnormalize {
    t_object x_obj;
    
};


static void list_vnormalize_list(t_list_vnormalize* x, t_symbol* s, int argc, t_atom* argv)
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

static void* list_vnormalize_new()
{
    t_list_vnormalize* x = reinterpret_cast<t_list_vnormalize*>(pd_new(list_vnormalize_class));
    outlet_new(&x->x_obj, &s_list);
    
    
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2evnormalize()
{
    list_vnormalize_class = class_new(gensym("list.vnormalize"),
                                  reinterpret_cast<t_newmethod>(list_vnormalize_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_list_vnormalize), 0, A_NULL);
    class_addlist(list_vnormalize_class, list_vnormalize_list);
    
    
    
}
