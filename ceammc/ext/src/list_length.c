#include <m_pd.h>
#include <math.h>

#include "ceammc.h"

t_class* length_class;

typedef struct length {
    t_object x_obj;
} t_length;

static void length_list(t_length* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_float(x->x_obj.te_outlet, argc);
}

static void* length_new()
{
    t_length* x = (t_length*)pd_new(length_class);
    outlet_new(&x->x_obj, &s_float);
    return (void*)x;
}

void CEAMMC_LIST_MODULE(length)
{
    length_class = class_new(CEAMMC_LIST_GENSYM(length),
        (t_newmethod)length_new, 0, sizeof(t_length), 0, A_NULL);
    class_addlist(length_class, length_list);
}
