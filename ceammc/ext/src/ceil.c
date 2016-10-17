#include <math.h>
#include <m_pd.h>

#include "ceammc.h"

t_class *ceil_class;

typedef struct ceil
{
    t_object x_ob;
    t_outlet * x_outlet;
} t_ceil;

void ceil_float(t_ceil *x, t_floatarg f)
{
#if PD_FLOATSIZE == 32
    outlet_float(x->x_outlet, ceilf(f));
#elif PD_FLOATSIZE == 64
    outlet_float(x->x_outlet, ceil(f));
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

void *ceil_new()
{
    t_ceil *x = (t_ceil*) pd_new(ceil_class);
    x->x_outlet = outlet_new(&x->x_ob, gensym("float"));
    return (void*) x;
}

void CEAMMC_MATH_MODULE(ceil)
{
    ceil_class = class_new(gensym(CEAMMC_MATH_EXT("ceil")),
                            (t_newmethod) ceil_new, 0,
                            sizeof(t_ceil), 0, A_NULL);
    class_addfloat(ceil_class, ceil_float);
}
