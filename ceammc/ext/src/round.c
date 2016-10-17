#include <math.h>
#include <m_pd.h>

#include "ceammc.h"

t_class *round_class;

typedef struct round
{
    t_object x_ob;
    t_outlet * x_outlet;
} t_round;

void round_float(t_round *x, t_floatarg f)
{
#if PD_FLOATSIZE == 32
    outlet_float(x->x_outlet, roundf(f));
#elif PD_FLOATSIZE == 64
    outlet_float(x->x_outlet, round(f));
#else
#error "Unsupported PD_FLOATSIZE"
#endif
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
                            sizeof(t_round), 0, A_NULL);
    class_addfloat(round_class, round_float);
}
