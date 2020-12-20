#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>

t_class* is_odd_class;
struct t_is_odd {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_odd_float(t_is_odd* x, t_floatarg f)
{
    if (static_cast<t_int>(f) % 2 == 1) {
        outlet_float(x->out_value, f);
        outlet_float(x->x_obj.te_outlet, 1);
    } else {
        outlet_float(x->x_obj.te_outlet, 0);
    }
}

static void* is_odd_new()
{
    t_is_odd* x = reinterpret_cast<t_is_odd*>(pd_new(is_odd_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

static void is_odd_free(t_is_odd* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_odd_setup()
{
    is_odd_class = class_new(gensym("is_odd"),
        reinterpret_cast<t_newmethod>(is_odd_new),
        reinterpret_cast<t_method>(is_odd_free),
        sizeof(t_is_odd), 0, A_NULL);
    class_doaddfloat(is_odd_class, reinterpret_cast<t_method>(is_odd_float));
}
