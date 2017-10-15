#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.trunc"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_trunc_class;
typedef struct math_trunc {
    t_object x_obj;
} t_math_trunc;

static t_float private_math_trunc(t_float v) 
{
#if PD_FLOATSIZE == 32
    return truncf(v);
#elif PD_FLOATSIZE == 64
    return trunc(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_trunc_float(t_math_trunc* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_trunc(f));
}

static void math_trunc_list(t_math_trunc* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_trunc);
}

static void* math_trunc_new()
{
    t_math_trunc* x = (t_math_trunc*)pd_new(math_trunc_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2etrunc()
{
    math_trunc_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_trunc_new, (t_method)0,
        sizeof(t_math_trunc), 0, A_NULL);
    class_addfloat(math_trunc_class, math_trunc_float);
    class_addlist(math_trunc_class, math_trunc_list);
}

