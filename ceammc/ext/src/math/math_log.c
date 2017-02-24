#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.log"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_log_class;
typedef struct math_log {
    t_object x_obj;
} t_math_log;

static t_float private_math_log(t_float v) 
{
#if PD_FLOATSIZE == 32
    return logf(v);
#elif PD_FLOATSIZE == 64
    return log(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_log_float(t_math_log* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_log(f));
}

static void math_log_list(t_math_log* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_log);
}

static void* math_log_new()
{
    t_math_log* x = (t_math_log*)pd_new(math_log_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2elog()
{
    math_log_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_log_new, (t_method)0,
        sizeof(t_math_log), 0, A_NULL);
    class_addfloat(math_log_class, math_log_float);
    class_addlist(math_log_class, math_log_list);
}

