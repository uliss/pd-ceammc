#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* pi_class;

typedef struct math_abs {
    t_object x_ob;
    t_outlet* x_outlet;
} t_pi;

void pi_bang(t_pi* x)
{
    outlet_float(x->x_outlet, M_PI);
}

void* pi_new()
{
    t_pi* x = (t_pi*)pd_new(pi_class);
    x->x_outlet = outlet_new(&x->x_ob, gensym("float"));
    return (void*)x;
}

void CEAMMC_MATH_MODULE(pi)
{
    pi_class = class_new(gensym(CEAMMC_MATH_EXT("pi")),
        (t_newmethod)pi_new, 0,
        sizeof(t_pi), 0, A_NULL);
    class_addbang(pi_class, pi_bang);
}
