#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* math_abs_class;

typedef struct math_abs {
    t_object x_ob;
    t_outlet* x_outlet;
} t_abs;

static t_float abs_private(float_t v)
{
#if PD_FLOATSIZE == 32
    return fabsf(v);
#elif PD_FLOATSIZE == 64
    return fabs(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif
}

void math_abs_float(t_abs* x, t_floatarg f)
{
    outlet_float(x->x_outlet, abs_private(f));
}

void math_abs_list(t_abs* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc_atoms_map_float_to_outlet(x->x_outlet, s, argc, argv, abs_private);
}

void* math_abs_new()
{
    t_abs* x = (t_abs*)pd_new(math_abs_class);
    x->x_outlet = outlet_new(&x->x_ob, gensym("float"));
    return (void*)x;
}

void CEAMMC_MATH_MODULE(abs)
{
    math_abs_class = class_new(gensym(CEAMMC_MATH_EXT("abs")),
        (t_newmethod)math_abs_new, 0,
        sizeof(t_abs), 0, A_NULL);
    class_addfloat(math_abs_class, math_abs_float);
    class_addlist(math_abs_class, math_abs_list);
}
