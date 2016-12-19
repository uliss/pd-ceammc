#include "ceammc.h"
#include <m_pd.h>
#include <glib.h>

t_class* path_basename_class;

struct t_path_basename {
    t_object x_ob;
    t_outlet* x_out;
};

static void path_basename_free(t_path_basename* x)
{
    outlet_free(x->x_out);
}

static void* path_basename_new()
{
    t_path_basename* x = reinterpret_cast<t_path_basename*>(pd_new(path_basename_class));
    x->x_out = outlet_new(&x->x_ob, &s_symbol);
    return static_cast<void*>(x);
}

static void path_basename_symbol(t_path_basename* x, t_symbol* s)
{
    gchar* bn = g_path_get_basename(s->s_name);
    outlet_symbol(x->x_out, gensym(bn));
    g_free(bn);
}

extern "C" void CEAMMC_DOT_MODULE(path, basename)
{
    path_basename_class = class_new(CEAMMC_PATH_GENSYM(basename),
        (t_newmethod)path_basename_new, (t_method)path_basename_free,
        sizeof(t_path_basename), 0, A_NULL);
    class_addsymbol(path_basename_class, path_basename_symbol);
}
