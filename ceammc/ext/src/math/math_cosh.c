#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.cosh"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_cosh_class;
typedef struct math_cosh {
    t_object x_obj;
} t_math_cosh;

static t_float private_math_cosh(t_float v) 
{
#if PD_FLOATSIZE == 32
    return coshf(v);
#elif PD_FLOATSIZE == 64
    return cosh(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_cosh_float(t_math_cosh* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_cosh(f));
}

static void math_cosh_list(t_math_cosh* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_cosh);
}

static void* math_cosh_new()
{
    t_math_cosh* x = (t_math_cosh*)pd_new(math_cosh_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2ecosh()
{
    math_cosh_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_cosh_new, (t_method)0,
        sizeof(t_math_cosh), 0, A_NULL);
    class_addfloat(math_cosh_class, math_cosh_float);
    class_addlist(math_cosh_class, math_cosh_list);
}

