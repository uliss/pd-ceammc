#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.log2"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_log2_class;
typedef struct math_log2 {
    t_object x_obj;
} t_math_log2;

static t_float private_math_log2(t_float v) 
{
#if PD_FLOATSIZE == 32
    return log2f(v);
#elif PD_FLOATSIZE == 64
    return log2(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_log2_float(t_math_log2* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_log2(f));
}

static void math_log2_list(t_math_log2* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_log2);
}

static void* math_log2_new()
{
    t_math_log2* x = (t_math_log2*)pd_new(math_log2_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2elog2()
{
    math_log2_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_log2_new, (t_method)0,
        sizeof(t_math_log2), 0, A_NULL);
    class_addfloat(math_log2_class, math_log2_float);
    class_addlist(math_log2_class, math_log2_list);
}

