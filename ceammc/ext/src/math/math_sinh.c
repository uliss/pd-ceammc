#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.sinh"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_sinh_class;
typedef struct math_sinh {
    t_object x_obj;
} t_math_sinh;

static t_float private_math_sinh(t_float v) 
{
#if PD_FLOATSIZE == 32
    return sinhf(v);
#elif PD_FLOATSIZE == 64
    return sinh(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_sinh_float(t_math_sinh* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_sinh(f));
}

static void math_sinh_list(t_math_sinh* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_sinh);
}

static void* math_sinh_new()
{
    t_math_sinh* x = (t_math_sinh*)pd_new(math_sinh_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2esinh()
{
    math_sinh_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_sinh_new, (t_method)0,
        sizeof(t_math_sinh), 0, A_NULL);
    class_addfloat(math_sinh_class, math_sinh_float);
    class_addlist(math_sinh_class, math_sinh_list);
}

