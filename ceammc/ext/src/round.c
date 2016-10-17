#include <math.h>
#include <m_pd.h>

t_class *round_class;

typedef struct round
{
    t_object x_ob;
    t_outlet * x_outlet;
} t_round;

void round_float(t_round *x, t_floatarg f)
{
    outlet_float(x->x_outlet, round(f));
}

void *round_new()
{
    t_round *x = (t_round*) pd_new(round_class);
    x->x_outlet = outlet_new(&x->x_ob, gensym("float"));
    return (void*) x;
}

void round_setup()
{
    round_class = class_new(gensym("round"),
                            (t_newmethod) round_new, 0,
                            sizeof(t_round), 0, 0);
    class_addfloat(round_class, round_float);
}
