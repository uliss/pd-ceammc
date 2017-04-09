#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* path_listdir_class;
struct t_path_listdir {
    t_object x_obj;
};

static void path_listdir_symbol(t_path_listdir* x, t_symbol* s)
{
    GDir* dir = g_dir_open(s->s_name, 0, NULL);
    if (dir == NULL)
        return;

    const gchar* name = NULL;
    while ((name = g_dir_read_name(dir)) != NULL) {
        outlet_symbol(x->x_obj.te_outlet, gensym(name));
    }

    g_dir_close(dir);
}

static void* path_listdir_new()
{
    t_path_listdir* x = reinterpret_cast<t_path_listdir*>(pd_new(path_listdir_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_path0x2elistdir()
{
    path_listdir_class = class_new(gensym("path.listdir"),
        reinterpret_cast<t_newmethod>(path_listdir_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_path_listdir), 0, A_NULL);
    class_addsymbol(path_listdir_class, path_listdir_symbol);
}
