#include "ceammc.hpp"
#include <m_pd.h>
#include <algorithm>

t_class* list_unique_class;
struct t_list_unique {
    t_object x_obj;
};

typedef ceammc::pd::atom_list::iterator iterator;

static void list_unique_list(t_list_unique* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    ceammc::pd::atom_list lst(argv, argv + argc);
    std::sort(lst.begin(), lst.end(), ceammc::pd::atoms_compare_lt);
    iterator last = std::unique(lst.begin(), lst.end(), ceammc::pd::atom_equals);
    lst.erase(last, lst.end());
    ceammc::pd::output(x->x_obj.te_outlet, lst);
}

static void* list_unique_new()
{
    t_list_unique* x = reinterpret_cast<t_list_unique*>(pd_new(list_unique_class));
    outlet_new(&x->x_obj, &s_float);

    return static_cast<void*>(x);
}

extern "C" void setup_list0x2eunique()
{
    list_unique_class = class_new(gensym("list.unique"),
        reinterpret_cast<t_newmethod>(list_unique_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_list_unique), 0, A_NULL);
    class_addlist(list_unique_class, list_unique_list);
}
