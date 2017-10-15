#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.atan"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_atan_class;
typedef struct math_atan {
    t_object x_obj;
} t_math_atan;

static t_float private_math_atan(t_float v) 
{
#if PD_FLOATSIZE == 32
    return atanf(v);
#elif PD_FLOATSIZE == 64
    return atan(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_atan_float(t_math_atan* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_atan(f));
}

static void math_atan_list(t_math_atan* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_atan);
}

static void* math_atan_new()
{
    t_math_atan* x = (t_math_atan*)pd_new(math_atan_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eatan()
{
    math_atan_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_atan_new, (t_method)0,
        sizeof(t_math_atan), 0, A_NULL);
    class_addfloat(math_atan_class, math_atan_float);
    class_addlist(math_atan_class, math_atan_list);
}

