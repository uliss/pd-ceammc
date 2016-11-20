#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* system_getenv_class;
struct t_system_getenv {
    t_object x_obj;
};

static void system_getenv_symbol(t_system_getenv* x, t_symbol* s)
{
    outlet_symbol(x->x_obj.te_outlet, gensym(g_getenv(s->s_name)));
}

static void* system_getenv_new()
{
    t_system_getenv* x = reinterpret_cast<t_system_getenv*>(pd_new(system_getenv_class));
    outlet_new(&x->x_obj, &s_float);

    return static_cast<void*>(x);
}

extern "C" void setup_system0x2egetenv()
{
    system_getenv_class = class_new(gensym("system.getenv"),
        reinterpret_cast<t_newmethod>(system_getenv_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_system_getenv), 0, A_NULL);
    class_addsymbol(system_getenv_class, system_getenv_symbol);
}
