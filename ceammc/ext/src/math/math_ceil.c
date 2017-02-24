#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.ceil"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_ceil_class;
typedef struct math_ceil {
    t_object x_obj;
} t_math_ceil;

static t_float private_math_ceil(t_float v) 
{
#if PD_FLOATSIZE == 32
    return ceilf(v);
#elif PD_FLOATSIZE == 64
    return ceil(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_ceil_float(t_math_ceil* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_ceil(f));
}

static void math_ceil_list(t_math_ceil* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_ceil);
}

static void* math_ceil_new()
{
    t_math_ceil* x = (t_math_ceil*)pd_new(math_ceil_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eceil()
{
    math_ceil_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_ceil_new, (t_method)0,
        sizeof(t_math_ceil), 0, A_NULL);
    class_addfloat(math_ceil_class, math_ceil_float);
    class_addlist(math_ceil_class, math_ceil_list);
}

