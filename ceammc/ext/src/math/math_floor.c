#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.floor"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_floor_class;
typedef struct math_floor {
    t_object x_obj;
} t_math_floor;

static t_float private_math_floor(t_float v) 
{
#if PD_FLOATSIZE == 32
    return floorf(v);
#elif PD_FLOATSIZE == 64
    return floor(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void math_floor_float(t_math_floor* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, private_math_floor(f));
}

static void math_floor_list(t_math_floor* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, private_math_floor);
}

static void* math_floor_new()
{
    t_math_floor* x = (t_math_floor*)pd_new(math_floor_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2efloor()
{
    math_floor_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_floor_new, (t_method)0,
        sizeof(t_math_floor), 0, A_NULL);
    class_addfloat(math_floor_class, math_floor_float);
    class_addlist(math_floor_class, math_floor_list);
}

