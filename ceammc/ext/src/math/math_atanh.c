#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.atanh"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_atanh_class;
typedef struct math_atanh {
    t_object x_obj;
} t_math_atanh;

static t_float private_math_atanh(t_float v) 
{
#if PD_FLOATSIZE == 32
    return atanhf(v);
#elif PD_FLOATSIZE == 64
    return atanh(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_atanh_float(t_math_atanh* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_atanh(f));
}

static void math_atanh_list(t_math_atanh* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_atanh);
}

static void* math_atanh_new()
{
    t_math_atanh* x = (t_math_atanh*)pd_new(math_atanh_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eatanh()
{
    math_atanh_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_atanh_new, (t_method)0,
        sizeof(t_math_atanh), 0, A_NULL);
    class_addfloat(math_atanh_class, math_atanh_float);
    class_addlist(math_atanh_class, math_atanh_list);
}

