#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

t_class* list_mean_class;
struct t_list_mean {
    t_object x_obj;
};

static t_float sum(t_float f1, t_float f2) { return f1 + f2; }

static void list_mean_list(t_list_mean* x, t_symbol* s, int argc, t_atom* argv)
{
    if(argc == 0) {
        return outlet_float(x->x_obj.te_outlet, NAN);
    }

    t_float mean = ceammc_atoms_reduce_float(argc, argv, 0.0, &sum) / argc;
    outlet_float(x->x_obj.te_outlet, mean);
}

static void* list_mean_new()
{
    t_list_mean* x = reinterpret_cast<t_list_mean*>(pd_new(list_mean_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2emean()
{
    list_mean_class = class_new(gensym("list.mean"),
        reinterpret_cast<t_newmethod>(list_mean_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_list_mean), 0, A_NULL);
    class_addlist(list_mean_class, list_mean_list);
}
