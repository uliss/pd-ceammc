#include "ceammc.h"
#include "m_pd.h"
#include <math.h>

#define OBJ_NAME "math.e"

static t_class* math_e_class;
struct t_math_e {
    t_object x_obj;
};

static void math_e_bang(t_math_e* x)
{
    outlet_float(x->x_obj.te_outlet, M_E);
}

static void* math_e_new()
{
    t_math_e* x = reinterpret_cast<t_math_e*>(pd_new(math_e_class));
    outlet_new(&x->x_obj, &s_float);
    
    return static_cast<void*>(x);
}

extern void setup_math_e()
{
    math_e_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(math_e_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_math_e), 0, A_NULL);
    class_addbang(math_e_class, reinterpret_cast<t_method>(math_e_bang));
}

