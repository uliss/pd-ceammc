#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.log10"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_log10_class;
typedef struct math_log10 {
    t_object x_obj;
} t_math_log10;

static t_float private_math_log10(t_float v) 
{
#if PD_FLOATSIZE == 32
    return log10f(v);
#elif PD_FLOATSIZE == 64
    return log10(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_log10_float(t_math_log10* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_log10(f));
}

static void math_log10_list(t_math_log10* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_log10);
}

static void* math_log10_new()
{
    t_math_log10* x = (t_math_log10*)pd_new(math_log10_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2elog10()
{
    math_log10_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_log10_new, (t_method)0,
        sizeof(t_math_log10), 0, A_NULL);
    class_addfloat(math_log10_class, math_log10_float);
    class_addlist(math_log10_class, math_log10_list);
}

