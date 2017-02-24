#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.exp"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_exp_class;
typedef struct math_exp {
    t_object x_obj;
} t_math_exp;

static t_float private_math_exp(t_float v) 
{
#if PD_FLOATSIZE == 32
    return expf(v);
#elif PD_FLOATSIZE == 64
    return exp(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_exp_float(t_math_exp* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_exp(f));
}

static void math_exp_list(t_math_exp* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_exp);
}

static void* math_exp_new()
{
    t_math_exp* x = (t_math_exp*)pd_new(math_exp_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eexp()
{
    math_exp_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_exp_new, (t_method)0,
        sizeof(t_math_exp), 0, A_NULL);
    class_addfloat(math_exp_class, math_exp_float);
    class_addlist(math_exp_class, math_exp_list);
}

