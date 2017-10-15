#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.tan"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_tan_class;
typedef struct math_tan {
    t_object x_obj;
} t_math_tan;

static t_float private_math_tan(t_float v) 
{
#if PD_FLOATSIZE == 32
    return tanf(v);
#elif PD_FLOATSIZE == 64
    return tan(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_tan_float(t_math_tan* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_tan(f));
}

static void math_tan_list(t_math_tan* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_tan);
}

static void* math_tan_new()
{
    t_math_tan* x = (t_math_tan*)pd_new(math_tan_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2etan()
{
    math_tan_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_tan_new, (t_method)0,
        sizeof(t_math_tan), 0, A_NULL);
    class_addfloat(math_tan_class, math_tan_float);
    class_addlist(math_tan_class, math_tan_list);
}

