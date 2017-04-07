#include <m_pd.h>
#include <glib.h>
#include "ceammc.h"

t_class* path_home_class;
struct t_path_home {
    t_object x_obj;
};

static void path_home_bang(t_path_home* x)
{
    outlet_symbol(x->x_obj.te_outlet, gensym(g_get_home_dir()));
}

static void* path_home_new()
{
    t_path_home* x = reinterpret_cast<t_path_home*>(pd_new(path_home_class));
    outlet_new(&x->x_obj, &s_float);   
    return static_cast<void*>(x);
}

extern "C" void setup_path0x2ehome()
{
    path_home_class = class_new(gensym("path.home"),
        reinterpret_cast<t_newmethod>(path_home_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_path_home), 0, A_NULL);
    class_addbang(path_home_class, path_home_bang);
}

