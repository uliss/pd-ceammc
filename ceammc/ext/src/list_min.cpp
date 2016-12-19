#include "ceammc.hpp"
#include <algorithm>
#include <m_pd.h>

t_class* list_min_class;
typedef struct list_min {
    t_object x_obj;
} t_list_min;

static void list_min_list(t_list_min* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc == 0)
        return;

    t_atom* a = std::min_element(argv, argv + argc, ceammc::pd::atoms_compare_lt);

    if (a->a_type == A_FLOAT)
        return outlet_float(x->x_obj.te_outlet, atom_getfloat(a));
    else if (a->a_type == A_SYMBOL)
        return outlet_symbol(x->x_obj.te_outlet, atom_getsymbol(a));
    else
        return pd_error(x, "only floats and symbols are supported");
}

static void* list_min_new()
{
    t_list_min* x = reinterpret_cast<t_list_min*>(pd_new(list_min_class));
    outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2emin()
{
    list_min_class = class_new(gensym("list.min"),
        (t_newmethod)list_min_new, (t_method)0,
        sizeof(t_list_min), 0, A_NULL);
    class_addlist(list_min_class, list_min_list);
}
