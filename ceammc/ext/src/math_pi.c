#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* pi_class;

typedef struct math_abs {
    t_object x_obj;
} t_pi;

static void pi_bang(t_pi* x)
{
    outlet_float(x->x_obj.te_outlet, M_PI);
}

static void* pi_new()
{
    t_pi* x = (t_pi*)pd_new(pi_class);
    outlet_new(&x->x_obj, &s_float);
    return (void*)x;
}

void CEAMMC_MATH_MODULE(pi)
{
    pi_class = class_new(CEAMMC_MATH_GENSYM(pi),
        (t_newmethod)pi_new, 0, sizeof(t_pi), 0, A_NULL);
    class_addbang(pi_class, pi_bang);
}
