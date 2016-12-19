#include "ceammc.h"
#include <m_pd.h>

t_class* system_memsize_class;
struct t_system_memsize {
    t_object x_obj;
};

static void system_memsize_bang(t_system_memsize* x)
{
    outlet_float(x->x_obj.te_outlet, ceammc_memory_size());
}

static void* system_memsize_new()
{
    t_system_memsize* x = reinterpret_cast<t_system_memsize*>(pd_new(system_memsize_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_system0x2ememsize()
{
    system_memsize_class = class_new(gensym("system.memsize"),
        reinterpret_cast<t_newmethod>(system_memsize_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_system_memsize), 0, A_NULL);
    class_addbang(system_memsize_class, system_memsize_bang);
}
