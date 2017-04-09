#include "ceammc.hpp"
#include <m_pd.h>

t_class* reject_this_class;
struct t_reject_this {
    t_object x_obj;
    ceammc::pd::atom_list* args;
};

static void reject_this_float(t_reject_this* x, t_floatarg f)
{
    if (!ceammc::pd::atom_list_contains(*x->args, f))
        outlet_float(x->x_obj.te_outlet, f);
}

static void reject_this_list(t_reject_this* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc::pd::atom_list passed_items;
    for (int i = 0; i < argc; i++) {
        if (!ceammc::pd::atom_list_contains(*x->args, argv[i]))
            passed_items.push_back(argv[i]);
    }

    if (!passed_items.empty())
        ceammc::pd::output(x->x_obj.te_outlet, passed_items);
}

static void reject_this_symbol(t_reject_this* x, t_symbol* s)
{
    if (!ceammc::pd::atom_list_contains(*x->args, s))
        outlet_symbol(x->x_obj.te_outlet, s);
}

static void* reject_this_new(t_symbol* s, int argc, t_atom* argv)
{
    t_reject_this* x = reinterpret_cast<t_reject_this*>(pd_new(reject_this_class));
    outlet_new(&x->x_obj, &s_float);

    x->args = new ceammc::pd::atom_list;
    for (int i = 0; i < argc; i++) {
        x->args->push_back(argv[i]);
    }

    return static_cast<void*>(x);
}

static void reject_this_free(t_reject_this* x)
{
    delete x->args;
}

extern "C" void setup_reject0x2ethis()
{
    reject_this_class = class_new(gensym("reject.this"),
        reinterpret_cast<t_newmethod>(reject_this_new),
        reinterpret_cast<t_method>(reject_this_free),
        sizeof(t_reject_this), 0, A_GIMME, 0);
    class_addfloat(reject_this_class, reject_this_float);
    class_addlist(reject_this_class, reject_this_list);
    class_addsymbol(reject_this_class, reject_this_symbol);
}
