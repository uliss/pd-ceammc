#include "ceammc.h"
#include <m_pd.h>
#include <sys/stat.h>

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
    struct stat buf;
    t_float val = (stat(s->s_name, &buf) == 0) ? 1 : 0;
    outlet_float(x->x_obj.ob_outlet, val);
}

extern "C" void CEAMMC_PATH_MODULE(exists)
{
    path_exists_class = class_new(CEAMMC_PATH_GENSYM(exists),
        (t_newmethod)path_exists_new, 0,
        sizeof(t_path_exists), 0, A_NULL);
    class_addsymbol(path_exists_class, path_exists_symbol);
}
