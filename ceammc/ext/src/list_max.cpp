#include "ceammc.hpp"
#include <algorithm>
#include <m_pd.h>

t_class* list_max_class;
typedef struct list_max {
    t_object x_obj;
} t_list_max;

static void list_max_list(t_list_max* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc == 0)
        return;

    t_atom* a = std::max_element(argv, argv + argc, ceammc::pd::atoms_compare_lt);

    if (a->a_type == A_FLOAT)
        return outlet_float(x->x_obj.te_outlet, atom_getfloat(a));
    else if (a->a_type == A_SYMBOL)
        return outlet_symbol(x->x_obj.te_outlet, atom_getsymbol(a));
    else
        return pd_error(x, "only floats and symbols are supported");
}

static void* list_max_new()
{
    t_list_max* x = (t_list_max*)pd_new(list_max_class);
    outlet_new(&x->x_obj, &s_anything);
    return (void*)x;
}

extern "C" void setup_list0x2emax()
{
    list_max_class = class_new(gensym("list.max"),
        (t_newmethod)list_max_new, (t_method)0,
        sizeof(t_list_max), 0, A_NULL);
    class_addlist(list_max_class, list_max_list);
}
