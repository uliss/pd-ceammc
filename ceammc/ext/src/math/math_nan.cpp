#include "ceammc.h"
#include "m_pd.h"
#include <math.h>

#define OBJ_NAME "math.nan"

static t_class* math_nan_class;
struct t_math_nan {
    t_object x_obj;
};

static void math_nan_bang(t_math_nan* x)
{
    outlet_float(x->x_obj.te_outlet, NAN);
}

static void* math_nan_new()
{
    t_math_nan* x = reinterpret_cast<t_math_nan*>(pd_new(math_nan_class));
    outlet_new(&x->x_obj, &s_float);
    
    return static_cast<void*>(x);
}

extern void setup_math_nan()
{
    math_nan_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(math_nan_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_math_nan), 0, A_NULL);
    class_addbang(math_nan_class, reinterpret_cast<t_method>(math_nan_bang));
}

