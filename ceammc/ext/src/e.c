#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* math_e_class;

typedef struct math_e {
    t_object x_ob;
    t_outlet* x_outlet;
} t_math_e;

void e_bang(t_math_e* x)
{
    outlet_float(x->x_outlet, M_E);
}

void* math_e_new()
{
    t_math_e* x = (t_math_e*)pd_new(math_e_class);
    x->x_outlet = outlet_new(&x->x_ob, gensym("float"));
    return (void*)x;
}

void CEAMMC_MATH_MODULE(e)
{
    math_e_class = class_new(gensym(CEAMMC_MATH_EXT("e")),
        (t_newmethod)math_e_new, 0,
        sizeof(t_math_e), 0, A_NULL);
    class_addbang(math_e_class, e_bang);
}
