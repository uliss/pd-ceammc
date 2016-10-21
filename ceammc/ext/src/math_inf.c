#include <m_pd.h>
#include <math.h>
#include "ceammc.h"

t_class* math_inf_class;
typedef struct math_inf {
    t_object x_obj;
} t_math_inf;

static void math_inf_bang(t_math_inf* x)
{
    outlet_float(x->x_obj.te_outlet, INFINITY);
}

static void* math_inf_new()
{
    t_math_inf* x = (t_math_inf*)pd_new(math_inf_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2einf()
{
    math_inf_class = class_new(gensym("math.inf"),
        (t_newmethod)math_inf_new, 0,
            sizeof(t_math_inf), 0, A_NULL);
    class_addbang(math_inf_class, math_inf_bang);
}

