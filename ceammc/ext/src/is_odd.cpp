#include <m_pd.h>
#include <stdlib.h>
#include "ceammc.h"

t_class* is_odd_class;
struct t_is_odd {
    t_object x_obj;
};

static void is_odd_float(t_is_odd* x, t_floatarg f)
{
    t_int v = static_cast<t_int>(f);
    outlet_float(x->x_obj.te_outlet, (labs(v) % 2 == 1) ? 1 : 0);
}

static void* is_odd_new()
{
    t_is_odd* x = reinterpret_cast<t_is_odd*>(pd_new(is_odd_class));
    outlet_new(&x->x_obj, &s_float);
    
    return static_cast<void*>(x);
}

extern "C" void setup_is0x2eodd()
{
    is_odd_class = class_new(gensym("is.odd"),
        static_cast<t_newmethod>(is_odd_new),
        static_cast<t_method>(0),
        sizeof(t_is_odd), 0, A_NULL);
    class_addfloat(is_odd_class, is_odd_float);
}

