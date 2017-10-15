#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.acosh"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_acosh_class;
typedef struct math_acosh {
    t_object x_obj;
} t_math_acosh;

static t_float private_math_acosh(t_float v) 
{
#if PD_FLOATSIZE == 32
    return acoshf(v);
#elif PD_FLOATSIZE == 64
    return acosh(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_acosh_float(t_math_acosh* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_acosh(f));
}

static void math_acosh_list(t_math_acosh* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_acosh);
}

static void* math_acosh_new()
{
    t_math_acosh* x = (t_math_acosh*)pd_new(math_acosh_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eacosh()
{
    math_acosh_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_acosh_new, (t_method)0,
        sizeof(t_math_acosh), 0, A_NULL);
    class_addfloat(math_acosh_class, math_acosh_float);
    class_addlist(math_acosh_class, math_acosh_list);
}

