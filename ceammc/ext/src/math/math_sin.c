#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.sin"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_sin_class;
typedef struct math_sin {
    t_object x_obj;
} t_math_sin;

static t_float private_math_sin(t_float v) 
{
#if PD_FLOATSIZE == 32
    return sinf(v);
#elif PD_FLOATSIZE == 64
    return sin(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_sin_float(t_math_sin* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_sin(f));
}

static void math_sin_list(t_math_sin* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_sin);
}

static void* math_sin_new()
{
    t_math_sin* x = (t_math_sin*)pd_new(math_sin_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2esin()
{
    math_sin_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_sin_new, (t_method)0,
        sizeof(t_math_sin), 0, A_NULL);
    class_addfloat(math_sin_class, math_sin_float);
    class_addlist(math_sin_class, math_sin_list);
}

