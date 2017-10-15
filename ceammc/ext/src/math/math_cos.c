#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.cos"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_cos_class;
typedef struct math_cos {
    t_object x_obj;
} t_math_cos;

static t_float private_math_cos(t_float v) 
{
#if PD_FLOATSIZE == 32
    return cosf(v);
#elif PD_FLOATSIZE == 64
    return cos(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_cos_float(t_math_cos* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_cos(f));
}

static void math_cos_list(t_math_cos* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_cos);
}

static void* math_cos_new()
{
    t_math_cos* x = (t_math_cos*)pd_new(math_cos_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2ecos()
{
    math_cos_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_cos_new, (t_method)0,
        sizeof(t_math_cos), 0, A_NULL);
    class_addfloat(math_cos_class, math_cos_float);
    class_addlist(math_cos_class, math_cos_list);
}

