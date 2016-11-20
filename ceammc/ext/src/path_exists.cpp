#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* path_exists_class;

struct t_path_exists {
    t_object x_obj;
};

static void* path_exists_new()
{
    t_path_exists* x = reinterpret_cast<t_path_exists*>(pd_new(path_exists_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

static void path_exists_symbol(t_path_exists* x, t_symbol* s)
{
    outlet_float(x->x_obj.ob_outlet, g_file_test(s->s_name, G_FILE_TEST_EXISTS) ? 1 : 0);
}

extern "C" void CEAMMC_PATH_MODULE(exists)
{
    path_exists_class = class_new(CEAMMC_PATH_GENSYM(exists),
        (t_newmethod)path_exists_new, 0,
        sizeof(t_path_exists), 0, A_NULL);
    class_addsymbol(path_exists_class, path_exists_symbol);
}
