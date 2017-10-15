#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.tanh"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_tanh_class;
typedef struct math_tanh {
    t_object x_obj;
} t_math_tanh;

static t_float private_math_tanh(t_float v) 
{
#if PD_FLOATSIZE == 32
    return tanhf(v);
#elif PD_FLOATSIZE == 64
    return tanh(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_tanh_float(t_math_tanh* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_tanh(f));
}

static void math_tanh_list(t_math_tanh* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_tanh);
}

static void* math_tanh_new()
{
    t_math_tanh* x = (t_math_tanh*)pd_new(math_tanh_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2etanh()
{
    math_tanh_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_tanh_new, (t_method)0,
        sizeof(t_math_tanh), 0, A_NULL);
    class_addfloat(math_tanh_class, math_tanh_float);
    class_addlist(math_tanh_class, math_tanh_list);
}

