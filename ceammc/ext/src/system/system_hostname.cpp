#include <m_pd.h>
#include <glib.h>
#include "ceammc.h"

static t_class* system_hostname_class;
struct t_system_hostname {
    t_object x_obj;
};

static void system_hostname_bang(t_system_hostname* x)
{
    outlet_symbol(x->x_obj.te_outlet, gensym(g_get_host_name()));
}

static void* system_hostname_new()
{
    t_system_hostname* x = reinterpret_cast<t_system_hostname*>(pd_new(system_hostname_class));
    outlet_new(&x->x_obj, &s_float);
    
    return static_cast<void*>(x);
}

extern "C" void setup_system0x2ehostname()
{
    system_hostname_class = class_new(gensym("system.hostname"),
        reinterpret_cast<t_newmethod>(system_hostname_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_system_hostname), 0, A_NULL);
    class_addbang(system_hostname_class, system_hostname_bang);
}

