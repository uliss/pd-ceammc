#include <m_pd.h>
#include <math.h>
#include "ceammc.h"

t_class* math_sign_class;
typedef struct math_sign {
    t_object x_obj;
} t_math_sign;

static t_float private_math_sign(t_float v) 
{
    if(v > 0.0)
        return 1;
    else if(v == 0.0)
        return 0;
    else
        return -1;
}

static void math_sign_float(t_math_sign* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_sign(f));
}

static void math_sign_list(t_math_sign* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_sign);
}

static void* math_sign_new()
{
    t_math_sign* x = (t_math_sign*)pd_new(math_sign_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2esign()
{
    math_sign_class = class_new(gensym("math.sign"),
        (t_newmethod)math_sign_new, 0,
            sizeof(t_math_sign), 0, A_NULL);
    class_addfloat(math_sign_class, math_sign_float);
    class_addlist(math_sign_class, math_sign_list);
}

