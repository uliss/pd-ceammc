#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.exp2"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_exp2_class;
typedef struct math_exp2 {
    t_object x_obj;
} t_math_exp2;

static t_float private_math_exp2(t_float v) 
{
#if PD_FLOATSIZE == 32
    return exp2f(v);
#elif PD_FLOATSIZE == 64
    return exp2(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_exp2_float(t_math_exp2* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_exp2(f));
}

static void math_exp2_list(t_math_exp2* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_exp2);
}

static void* math_exp2_new()
{
    t_math_exp2* x = (t_math_exp2*)pd_new(math_exp2_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eexp2()
{
    math_exp2_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_exp2_new, (t_method)0,
        sizeof(t_math_exp2), 0, A_NULL);
    class_addfloat(math_exp2_class, math_exp2_float);
    class_addlist(math_exp2_class, math_exp2_list);
}

