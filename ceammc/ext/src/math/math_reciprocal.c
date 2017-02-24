#include <m_pd.h>
#include <math.h>
#include "ceammc.h"

t_class* math_reciprocal_class;
typedef struct math_reciprocal {
    t_object x_obj;
} t_math_reciprocal;

static t_float private_math_reciprocal(t_float v) 
{
    return v == 0.0 ? NAN : (t_float)(1.0 / v);
}

static void math_reciprocal_float(t_math_reciprocal* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_reciprocal(f));
}

static void math_reciprocal_list(t_math_reciprocal* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_reciprocal);
}

static void* math_reciprocal_new()
{
    t_math_reciprocal* x = (t_math_reciprocal*)pd_new(math_reciprocal_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2ereciprocal()
{
    math_reciprocal_class = class_new(gensym("math.reciprocal"),
        (t_newmethod)math_reciprocal_new, 0,
            sizeof(t_math_reciprocal), 0, A_NULL);
    class_addfloat(math_reciprocal_class, math_reciprocal_float);
    class_addlist(math_reciprocal_class, math_reciprocal_list);
}

