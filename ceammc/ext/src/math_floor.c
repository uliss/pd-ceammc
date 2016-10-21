#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* floor_class;

typedef struct floor {
    t_object x_obj;
} t_floor;

static t_float floor_private(float_t v)
{
#if PD_FLOATSIZE == 32
    return floorf(v);
#elif PD_FLOATSIZE == 64
    return floor(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

static void floor_float(t_floor* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, floor_private(f));
}

static void floor_list(t_floor* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_obj.te_outlet, s, argc, argv, floor_private);
}

static void* floor_new()
{
    t_floor* x = (t_floor*)pd_new(floor_class);
    outlet_new(&x->x_obj, &s_float);
    return (void*)x;
}

void CEAMMC_MATH_MODULE(floor)
{
    floor_class = class_new(CEAMMC_MATH_GENSYM(floor),
        (t_newmethod)floor_new, 0,
        sizeof(t_floor), 0, A_NULL);
    class_addfloat(floor_class, floor_float);
    class_addlist(floor_class, floor_list);
}
