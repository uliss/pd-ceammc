#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.asin"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_asin_class;
typedef struct math_asin {
    t_object x_obj;
} t_math_asin;

static t_float private_math_asin(t_float v) 
{
#if PD_FLOATSIZE == 32
    return asinf(v);
#elif PD_FLOATSIZE == 64
    return asin(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_asin_float(t_math_asin* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_asin(f));
}

static void math_asin_list(t_math_asin* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_asin);
}

static void* math_asin_new()
{
    t_math_asin* x = (t_math_asin*)pd_new(math_asin_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2easin()
{
    math_asin_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_asin_new, (t_method)0,
        sizeof(t_math_asin), 0, A_NULL);
    class_addfloat(math_asin_class, math_asin_float);
    class_addlist(math_asin_class, math_asin_list);
}

