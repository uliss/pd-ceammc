#include <m_pd.h>
#include <math.h>
#include "ceammc.h"

t_class* math_squared_class;
typedef struct math_squared {
    t_object x_obj;
} t_math_squared;

static t_float private_math_squared(t_float v) 
{
    return v * v;
}

static void math_squared_float(t_math_squared* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_squared(f));
}

static void math_squared_list(t_math_squared* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_squared);
}

static void* math_squared_new()
{
    t_math_squared* x = (t_math_squared*)pd_new(math_squared_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2esquared()
{
    math_squared_class = class_new(gensym("math.squared"),
        (t_newmethod)math_squared_new, (t_method)0,
            sizeof(t_math_squared), 0, A_NULL);
    class_addfloat(math_squared_class, math_squared_float);
    class_addlist(math_squared_class, math_squared_list);
}

