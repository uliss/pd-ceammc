#include "ceammc_platform.h"

static t_class* system_memused_class;
struct t_system_memused {
    t_object x_obj;
    t_outlet* x_peak;
};

static void system_memused_bang(t_system_memused* x)
{
    outlet_float(x->x_peak, ceammc::platform::memory_peak_rss());
    outlet_float(x->x_obj.te_outlet, ceammc::platform::memory_current_rss());
}

static void* system_memused_new()
{
    t_system_memused* x = reinterpret_cast<t_system_memused*>(pd_new(system_memused_class));
    outlet_new(&x->x_obj, &s_float);
    x->x_peak = outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

static void system_memused_free(t_system_memused* x)
{
    outlet_free(x->x_peak);
}

void setup_system_memused()
{
    system_memused_class = class_new(gensym("system.memused"),
        reinterpret_cast<t_newmethod>(system_memused_new),
        reinterpret_cast<t_method>(system_memused_free),
        sizeof(t_system_memused), 0, A_NULL);
    class_addbang(system_memused_class, reinterpret_cast<t_method>(system_memused_bang));
}
