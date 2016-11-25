#include "ceammc.hpp"
#include <m_pd.h>

t_class* list_equal_class;
struct t_list_equal {
    t_object x_obj;
    ceammc::pd::atom_list* lst;
};

static void list_equal_list(t_list_equal* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc != x->lst->size()) {
        outlet_float(x->x_obj.te_outlet, 0);
        return;
    }

    for (int i = 0; i < argc; i++) {
        if (ceammc_atoms_equal(&argv[i], &x->lst->at(i)) == 0) {
            outlet_float(x->x_obj.te_outlet, 0);
            return;
        }
    }

    outlet_float(x->x_obj.te_outlet, 1);
}

static void* list_equal_new(t_symbol* s, int argc, t_atom* argv)
{
    t_list_equal* x = reinterpret_cast<t_list_equal*>(pd_new(list_equal_class));
    outlet_new(&x->x_obj, &s_float);
    x->lst = new ceammc::pd::atom_list(argv, argv + argc);
    return static_cast<void*>(x);
}

static void list_equal_free(t_list_equal* x)
{
    delete x->lst;
}

extern "C" void setup_list0x2eequal()
{
    list_equal_class = class_new(gensym("list.equal"),
        reinterpret_cast<t_newmethod>(list_equal_new),
        reinterpret_cast<t_method>(list_equal_free),
        sizeof(t_list_equal), CLASS_DEFAULT, A_GIMME, A_NULL);
    class_addlist(list_equal_class, list_equal_list);
}
