#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.acos"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_acos_class;
typedef struct math_acos {
    t_object x_obj;
} t_math_acos;

static t_float private_math_acos(t_float v) 
{
#if PD_FLOATSIZE == 32
    return acosf(v);
#elif PD_FLOATSIZE == 64
    return acos(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_acos_float(t_math_acos* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_acos(f));
}

static void math_acos_list(t_math_acos* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_acos);
}

static void* math_acos_new()
{
    t_math_acos* x = (t_math_acos*)pd_new(math_acos_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eacos()
{
    math_acos_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_acos_new, (t_method)0,
        sizeof(t_math_acos), 0, A_NULL);
    class_addfloat(math_acos_class, math_acos_float);
    class_addlist(math_acos_class, math_acos_list);
}

