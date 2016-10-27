#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* unpack_class;

typedef struct list_unpack {
    t_object x_obj;
    t_outlet* out_n;
} t_list_unpack;

static void unpack_anything(t_list_unpack* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_float(x->out_n, argc);

    for (int i = 0; i < argc; i++) {
        outlet_anything(x->x_obj.te_outlet, s, 1, argv + i);
    }
}

static void* unpack_new()
{
    t_list_unpack* x = (t_list_unpack*)pd_new(unpack_class);
    outlet_new(&x->x_obj, &s_anything);
    x->out_n = outlet_new(&x->x_obj, &s_float);
    return (void*)x;
}

static void unpack_free(t_list_unpack* x)
{
    outlet_free(x->out_n);
}

void CEAMMC_LIST_MODULE(unpack)
{
    unpack_class = class_new(CEAMMC_LIST_GENSYM(unpack),
        (t_newmethod)unpack_new, (t_method) unpack_free,
        sizeof(t_list_unpack), 0, A_NULL);
    class_addanything(unpack_class, unpack_anything);
}
