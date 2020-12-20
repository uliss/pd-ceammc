#include <m_pd.h>

t_class* is_even_class;
struct t_is_even {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_even_float(t_is_even* x, t_floatarg f)
{
    if (static_cast<t_int>(f) % 2 == 0) {
        outlet_float(x->out_value, f);
        outlet_float(x->x_obj.te_outlet, 1);
    } else {
        outlet_float(x->x_obj.te_outlet, 0);
    }
}

static void* is_even_new()
{
    t_is_even* x = reinterpret_cast<t_is_even*>(pd_new(is_even_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

static void is_even_free(t_is_even* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_even_setup()
{
    is_even_class = class_new(gensym("is_even"),
        static_cast<t_newmethod>(is_even_new),
        reinterpret_cast<t_method>(is_even_free),
        sizeof(t_is_even), 0, A_NULL);
    class_doaddfloat(is_even_class, reinterpret_cast<t_method>(is_even_float));
}
