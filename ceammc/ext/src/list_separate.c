#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* separate_class;

typedef struct separate {
    t_object x_ob;
    t_outlet* x_outlet;
} t_separate;

void separate_list(t_separate* x, t_symbol* s, int argc, t_atom* argv)
{
    for (int i = 0; i < argc; i++) {
        outlet_anything(x->x_outlet, s, 1, argv + i);
    }
}

void* separate_new()
{
    t_separate* x = (t_separate*)pd_new(separate_class);
    x->x_outlet = outlet_new(&x->x_ob, gensym("list"));
    return (void*)x;
}

void CEAMMC_LIST_MODULE(separate)
{
    separate_class = class_new(gensym(CEAMMC_LIST_EXT("separate")),
        (t_newmethod)separate_new, 0,
        sizeof(t_separate), 0, A_NULL);
    class_addlist(separate_class, separate_list);
}
