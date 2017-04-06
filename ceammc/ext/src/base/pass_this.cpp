#include "ceammc.hpp"
#include <m_pd.h>

t_class* pass_this_class;
struct t_pass_this {
    t_object x_obj;
    ceammc::pd::atom_list* args;
};

static void pass_float(t_pass_this* x, t_floatarg f)
{
    if (ceammc::pd::atom_list_contains(*x->args, f))
        outlet_float(x->x_obj.te_outlet, f);
}

static void pass_list(t_pass_this* x, t_symbol* s, int argc, t_atom* argv)
{
    ceammc::pd::atom_list passed_items;
    for (int i = 0; i < argc; i++) {
        if (ceammc::pd::atom_list_contains(*x->args, argv[i]))
            passed_items.push_back(argv[i]);
    }

    if (!passed_items.empty())
        ceammc::pd::output(x->x_obj.te_outlet, passed_items);
}

static void pass_symbol(t_pass_this* x, t_symbol* s)
{
    if (ceammc::pd::atom_list_contains(*x->args, s))
        outlet_symbol(x->x_obj.te_outlet, s);
}

static void* pass_new(t_symbol* s, int argc, t_atom* argv)
{
    t_pass_this* x = reinterpret_cast<t_pass_this*>(pd_new(pass_this_class));
    outlet_new(&x->x_obj, &s_float);

    x->args = new ceammc::pd::atom_list;
    for (int i = 0; i < argc; i++) {
        x->args->push_back(argv[i]);
    }

    return static_cast<void*>(x);
}

static void pass_free(t_pass_this* x)
{
    delete x->args;
}

extern "C" void setup_pass0x2ethis()
{
    pass_this_class = class_new(gensym("pass.this"),
        reinterpret_cast<t_newmethod>(pass_new),
        reinterpret_cast<t_method>(pass_free),
        sizeof(t_pass_this), 0, A_GIMME, 0);
    class_addfloat(pass_this_class, pass_float);
    class_addlist(pass_this_class, pass_list);
    class_addsymbol(pass_this_class, pass_symbol);
}
