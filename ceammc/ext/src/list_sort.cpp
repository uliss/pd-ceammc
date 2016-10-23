#include "ceammc.hpp"
#include <m_pd.h>
#include <algorithm>

t_class* list_sort_class;
struct t_list_sort {
    t_object x_obj;
};

static void list_sort_list(t_list_sort* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    ceammc::pd::atom_list lst(argv, argv + argc);
    std::sort(lst.begin(), lst.end(), ceammc::pd::atoms_compare_lt);
    ceammc::pd::output(x->x_obj.te_outlet, lst);
}

static void* list_sort_new()
{
    t_list_sort* x = reinterpret_cast<t_list_sort*>(pd_new(list_sort_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2esort()
{
    list_sort_class = class_new(gensym("list.sort"),
        static_cast<t_newmethod>(list_sort_new),
        static_cast<t_method>(0),
        sizeof(t_list_sort), 0, A_NULL);
    class_addlist(list_sort_class, list_sort_list);
}
