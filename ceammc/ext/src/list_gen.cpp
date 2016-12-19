#include "ceammc.hpp"
#include <m_pd.h>

t_class* list_gen_class;
struct t_list_gen {
    t_object x_obj;
    t_outlet* out_gen;
    t_inlet* in_gen;
    ceammc::pd::atom_list* values;
};

static void add_generated_value(t_list_gen* x, t_symbol* /*s*/, int argc, t_atom* argv)
{
    if (!x->values)
        return;

    for (int i = 0; i < argc; i++)
        x->values->push_back(argv[i]);
}

static void list_gen_float(t_list_gen* x, t_floatarg num)
{
    const int n = static_cast<int>(num);
    if (n < 1) {
        pd_error(x, "argument should be > 1. %i", n);
        return;
    }

    x->values = new ceammc::pd::atom_list();
    x->values->reserve(n);

    // output bangs to generator right outlet
    for (int i = 0; i < n; i++)
        outlet_bang(x->out_gen);

    // output generated values to main left outlet
    ceammc::pd::output(x->x_obj.te_outlet, *x->values);

    delete x->values;
    x->values = 0;
}

static void* list_gen_new(t_floatarg num)
{
    t_list_gen* x = reinterpret_cast<t_list_gen*>(pd_new(list_gen_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_gen = outlet_new(&x->x_obj, &s_bang);
    x->in_gen = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("generated value"));
    x->values = 0;
    return static_cast<void*>(x);
}

static void list_gen_free(t_list_gen* x)
{
    outlet_free(x->out_gen);
    inlet_free(x->in_gen);
}

extern "C" void setup_list0x2egen()
{
    list_gen_class = class_new(gensym("list.gen"),
        reinterpret_cast<t_newmethod>(list_gen_new),
        reinterpret_cast<t_method>(list_gen_free),
        sizeof(t_list_gen), 0, A_NULL);
    class_addfloat(list_gen_class, list_gen_float);
    class_addmethod(list_gen_class,
        reinterpret_cast<t_method>(add_generated_value),
        gensym("generated value"), A_GIMME, 0);
}
