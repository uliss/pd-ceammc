#include "ceammc.hpp"
#include <m_pd.h>

t_class* is_even_class;
struct t_is_even {
    t_object x_obj;
};

static void is_even_float(t_is_even* x, t_floatarg f)
{
    t_int v = static_cast<t_int>(f);
    outlet_float(x->x_obj.te_outlet, (v % 2 == 0) ? 1 : 0);
}

static void* is_even_new()
{
    t_is_even* x = reinterpret_cast<t_is_even*>(pd_new(is_even_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_is0x2eeven()
{
    is_even_class = class_new(gensym("is.even"),
        static_cast<t_newmethod>(is_even_new),
        static_cast<t_method>(0),
        sizeof(t_is_even), 0, A_NULL);
    class_addfloat(is_even_class, is_even_float);
}
