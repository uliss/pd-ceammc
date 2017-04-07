#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* path_dirname_class;
struct t_path_dirname {
    t_object x_obj;
};

static void path_dirname_symbol(t_path_dirname* x, t_symbol* s)
{
    gchar* bn = g_path_get_dirname(s->s_name);
    outlet_symbol(x->x_obj.te_outlet, gensym(bn));
    g_free(bn);
}

static void* path_dirname_new()
{
    t_path_dirname* x = reinterpret_cast<t_path_dirname*>(pd_new(path_dirname_class));
    outlet_new(&x->x_obj, &s_float);

    return static_cast<void*>(x);
}

extern "C" void setup_path0x2edirname()
{
    path_dirname_class = class_new(gensym("path.dirname"),
        reinterpret_cast<t_newmethod>(path_dirname_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_path_dirname), 0, A_NULL);
    class_addsymbol(path_dirname_class, path_dirname_symbol);
}
