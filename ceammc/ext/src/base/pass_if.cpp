#include "ceammc.hpp"
#include <m_pd.h>

static t_class* pass_if_class;
struct t_pass_if {
    t_object x_obj;
    t_inlet* in_pred;
    t_outlet* out_pred;
    t_float do_pass;
    t_atom value;
};

static void pass_if_float(t_pass_if* x, t_floatarg f)
{
    outlet_float(x->out_pred, f);

    if (x->do_pass != 0)
        outlet_float(x->x_obj.te_outlet, f);
}

static void pass_if_list(t_pass_if* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_list(x->out_pred, s, argc, argv);

    if (x->do_pass != 0)
        outlet_list(x->x_obj.te_outlet, s, argc, argv);
}

static void pass_if_symbol(t_pass_if* x, t_symbol* s)
{
    outlet_symbol(x->out_pred, s);

    if (x->do_pass != 0)
        outlet_symbol(x->x_obj.te_outlet, s);
}

static void pass_if_any(t_pass_if* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_anything(x->out_pred, s, argc, argv);

    if (x->do_pass != 0)
        outlet_anything(x->x_obj.te_outlet, s, argc, argv);
}

static void* pass_if_new()
{
    t_pass_if* x = reinterpret_cast<t_pass_if*>(pd_new(pass_if_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_pred = outlet_new(&x->x_obj, &s_anything);
    x->in_pred = floatinlet_new(&x->x_obj, &x->do_pass);
    x->do_pass = 1.0;
    return static_cast<void*>(x);
}

static void pass_if_free(t_pass_if* x)
{
    inlet_free(x->in_pred);
    outlet_free(x->out_pred);
}

extern "C" void setup_pass0x2eif()
{
    pass_if_class = class_new(gensym("pass.if"),
        reinterpret_cast<t_newmethod>(pass_if_new),
        reinterpret_cast<t_method>(pass_if_free),
        sizeof(t_pass_if), 0, A_NULL);
    class_addfloat(pass_if_class, pass_if_float);
    class_addlist(pass_if_class, pass_if_list);
    class_addsymbol(pass_if_class, pass_if_symbol);
    class_addanything(pass_if_class, pass_if_any);
}
