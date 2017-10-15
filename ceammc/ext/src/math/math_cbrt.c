#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.cbrt"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_cbrt_class;
typedef struct math_cbrt {
    t_object x_obj;
} t_math_cbrt;

static t_float private_math_cbrt(t_float v) 
{
#if PD_FLOATSIZE == 32
    return cbrtf(v);
#elif PD_FLOATSIZE == 64
    return cbrt(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_cbrt_float(t_math_cbrt* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_cbrt(f));
}

static void math_cbrt_list(t_math_cbrt* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_cbrt);
}

static void* math_cbrt_new()
{
    t_math_cbrt* x = (t_math_cbrt*)pd_new(math_cbrt_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2ecbrt()
{
    math_cbrt_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_cbrt_new, (t_method)0,
        sizeof(t_math_cbrt), 0, A_NULL);
    class_addfloat(math_cbrt_class, math_cbrt_float);
    class_addlist(math_cbrt_class, math_cbrt_list);
}

