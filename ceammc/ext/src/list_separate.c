#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* separate_class;

typedef struct separate {
    t_object x_obj;
    t_outlet* out_n;
} t_separate;

static void separate_anything(t_separate* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_float(x->out_n, argc);

    for (int i = 0; i < argc; i++) {
        outlet_anything(x->x_obj.te_outlet, s, 1, argv + i);
    }
}

static void* separate_new()
{
    t_separate* x = (t_separate*)pd_new(separate_class);
    outlet_new(&x->x_obj, &s_anything);
    x->out_n = outlet_new(&x->x_obj, &s_float);
    return (void*)x;
}

static void separate_free(t_separate* x)
{
    outlet_free(x->out_n);
}

void CEAMMC_LIST_MODULE(separate)
{
    separate_class = class_new(CEAMMC_LIST_GENSYM(separate),
        (t_newmethod)separate_new, (t_method) separate_free,
        sizeof(t_separate), 0, A_NULL);
    class_addanything(separate_class, separate_anything);
}
