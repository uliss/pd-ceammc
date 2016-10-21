#include <math.h>
#include <m_pd.h>

#include "ceammc.h"

t_class *round_class;

typedef struct round
{
    t_object x_ob;
    t_outlet * x_outlet;
} t_round;

static t_floatarg round_private(t_floatarg v) {
#if PD_FLOATSIZE == 32
    return roundf(v);
#elif PD_FLOATSIZE == 64
    return round(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

void round_float(t_round *x, t_floatarg f)
{
    outlet_float(x->x_outlet, round_private(f));
}

void round_list(t_round *x, t_symbol *s, int argc, t_atom *argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_outlet, s, argc, argv, round_private);
}

void *round_new()
{
    t_round *x = (t_round*) pd_new(round_class);
    x->x_outlet = outlet_new(&x->x_ob, gensym("float"));
    return (void*) x;
}

void CEAMMC_MATH_MODULE(round)
{
    round_class = class_new(gensym(CEAMMC_MATH_EXT("round")),
                            (t_newmethod) round_new, 0,
                            sizeof(t_round), CLASS_DEFAULT, A_NULL);
    class_addfloat(round_class, round_float);
    class_addlist(round_class, round_list);
}
