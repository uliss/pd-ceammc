#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.asinh"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_asinh_class;
typedef struct math_asinh {
    t_object x_obj;
} t_math_asinh;

static t_float private_math_asinh(t_float v) 
{
#if PD_FLOATSIZE == 32
    return asinhf(v);
#elif PD_FLOATSIZE == 64
    return asinh(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_asinh_float(t_math_asinh* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_asinh(f));
}

static void math_asinh_list(t_math_asinh* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_asinh);
}

static void* math_asinh_new()
{
    t_math_asinh* x = (t_math_asinh*)pd_new(math_asinh_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2easinh()
{
    math_asinh_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_asinh_new, (t_method)0,
        sizeof(t_math_asinh), 0, A_NULL);
    class_addfloat(math_asinh_class, math_asinh_float);
    class_addlist(math_asinh_class, math_asinh_list);
}

