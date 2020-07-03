#include "ceammc.h"
#include "m_pd.h"
#include <math.h>

#define OBJ_NAME "math.inf"

static t_class* math_inf_class;
struct t_math_inf {
    t_object x_obj;
};

static void math_inf_bang(t_math_inf* x)
{
    outlet_float(x->x_obj.te_outlet, INFINITY);
}

static void* math_inf_new()
{
    t_math_inf* x = reinterpret_cast<t_math_inf*>(pd_new(math_inf_class));
    outlet_new(&x->x_obj, &s_float);
    
    return static_cast<void*>(x);
}

extern void setup_math_inf()
{
    math_inf_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(math_inf_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_math_inf), 0, A_NULL);
    class_addbang(math_inf_class, reinterpret_cast<t_method>(math_inf_bang));
}

