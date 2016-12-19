#include "ceammc.hpp"
#include <algorithm>
#include <m_pd.h>

t_class* list_minmax_class;
struct t_list_minmax {
    t_object x_obj;
};

static void list_minmax_list(t_list_minmax* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc == 0)
        return;

    t_atom tuple[2];
    tuple[0] = *std::min_element(argv, argv + argc, ceammc::pd::atoms_compare_lt);
    tuple[1] = *std::max_element(argv, argv + argc, ceammc::pd::atoms_compare_lt);

    outlet_list(x->x_obj.te_outlet, &s_list, 2, (t_atom*)tuple);
}

static void* list_minmax_new()
{
    t_list_minmax* x = reinterpret_cast<t_list_minmax*>(pd_new(list_minmax_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2eminmax()
{
    list_minmax_class = class_new(gensym("list.minmax"),
        static_cast<t_newmethod>(list_minmax_new),
        static_cast<t_method>(0),
        sizeof(t_list_minmax), 0, A_NULL);
    class_addlist(list_minmax_class, list_minmax_list);
}
