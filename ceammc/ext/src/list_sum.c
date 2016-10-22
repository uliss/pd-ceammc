#include <m_pd.h>
#include "ceammc.h"

t_class* list_sum_class;
typedef struct list_sum {
    t_object x_obj;
} t_list_sum;

static void list_sum_float(t_list_sum* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, f);
}

static t_float sum(t_float f1, t_float f2) { return f1 + f2; }

static void list_sum_list(t_list_sum* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_float(x->x_obj.te_outlet, ceammc_atoms_reduce_float(argc, argv, &sum));
}

static void* list_sum_new()
{
    t_list_sum* x = (t_list_sum*)pd_new(list_sum_class);
    outlet_new(&x->x_obj, &s_float);
    return (void*)x;
}

void setup_list0x2esum()
{
    list_sum_class = class_new(gensym("list.sum"),
        (t_newmethod)list_sum_new, (t_method)0,
            sizeof(t_list_sum), 0, A_NULL);
    class_addfloat(list_sum_class, list_sum_float);
    class_addlist(list_sum_class, list_sum_list);
}

