#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.sqrt"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_sqrt_class;
typedef struct math_sqrt {
    t_object x_obj;
} t_math_sqrt;

static t_float private_math_sqrt(t_float v) 
{
#if PD_FLOATSIZE == 32
    return sqrtf(v);
#elif PD_FLOATSIZE == 64
    return sqrt(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_sqrt_float(t_math_sqrt* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_sqrt(f));
}

static void math_sqrt_list(t_math_sqrt* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_sqrt);
}

static void* math_sqrt_new()
{
    t_math_sqrt* x = (t_math_sqrt*)pd_new(math_sqrt_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2esqrt()
{
    math_sqrt_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_sqrt_new, (t_method)0,
        sizeof(t_math_sqrt), 0, A_NULL);
    class_addfloat(math_sqrt_class, math_sqrt_float);
    class_addlist(math_sqrt_class, math_sqrt_list);
}

