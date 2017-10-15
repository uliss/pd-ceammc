#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.abs"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_abs_class;
typedef struct math_abs {
    t_object x_obj;
} t_math_abs;

static t_float private_math_abs(t_float v) 
{
#if PD_FLOATSIZE == 32
    return fabsf(v);
#elif PD_FLOATSIZE == 64
    return fabs(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_abs_float(t_math_abs* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_abs(f));
}

static void math_abs_list(t_math_abs* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_abs);
}

static void* math_abs_new()
{
    t_math_abs* x = (t_math_abs*)pd_new(math_abs_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eabs()
{
    math_abs_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_abs_new, (t_method)0,
        sizeof(t_math_abs), 0, A_NULL);
    class_addfloat(math_abs_class, math_abs_float);
    class_addlist(math_abs_class, math_abs_list);
}

