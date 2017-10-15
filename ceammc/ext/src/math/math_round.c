#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.round"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_round_class;
typedef struct math_round {
    t_object x_obj;
} t_math_round;

static t_float private_math_round(t_float v) 
{
#if PD_FLOATSIZE == 32
    return roundf(v);
#elif PD_FLOATSIZE == 64
    return round(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_round_float(t_math_round* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_round(f));
}

static void math_round_list(t_math_round* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_round);
}

static void* math_round_new()
{
    t_math_round* x = (t_math_round*)pd_new(math_round_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2eround()
{
    math_round_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_round_new, (t_method)0,
        sizeof(t_math_round), 0, A_NULL);
    class_addfloat(math_round_class, math_round_float);
    class_addlist(math_round_class, math_round_list);
}

