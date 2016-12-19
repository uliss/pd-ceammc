#include "ceammc.hpp"
#include <m_pd.h>

t_class* list_normalize_class;
struct t_list_normalize {
    t_object x_obj;
};

static void list_normalize_list(t_list_normalize* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    // make a copy
    ceammc::pd::atom_list lst(argv, argv + argc);
    if (!ceammc::pd::atoms_normalize(lst))
        return;

    // output values
    ceammc::pd::output(x->x_obj.te_outlet, lst);
}

static void* list_normalize_new()
{
    t_list_normalize* x = reinterpret_cast<t_list_normalize*>(pd_new(list_normalize_class));
    outlet_new(&x->x_obj, &s_list);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2enormalize()
{
    list_normalize_class = class_new(gensym("list.normalize"),
        reinterpret_cast<t_newmethod>(list_normalize_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_list_normalize), 0, A_NULL);
    class_addlist(list_normalize_class, list_normalize_list);
}
