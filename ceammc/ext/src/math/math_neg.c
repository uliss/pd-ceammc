#include <m_pd.h>
#include <math.h>
#include "ceammc.h"

t_class* math_neg_class;
typedef struct math_neg {
    t_object x_obj;
} t_math_neg;

static t_float private_math_neg(t_float v) 
{
    return -1.0 * v;
}

static void math_neg_float(t_math_neg* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_neg(f));
}

static void math_neg_list(t_math_neg* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_neg);
}

static void* math_neg_new()
{
    t_math_neg* x = (t_math_neg*)pd_new(math_neg_class);
    outlet_new(&x->x_obj, &s_float);
    return (void*)x;
}

void setup_math0x2eneg()
{
    math_neg_class = class_new(gensym("math.neg"),
        (t_newmethod)math_neg_new, (t_method)0,
            sizeof(t_math_neg), 0, A_NULL);
    class_addfloat(math_neg_class, math_neg_float);
    class_addlist(math_neg_class, math_neg_list);
}

