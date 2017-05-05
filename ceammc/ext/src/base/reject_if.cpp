#include "ceammc.hpp"
#include <m_pd.h>

t_class* reject_if_class;
struct t_reject_if {
    t_object x_obj;
    t_inlet* in_pred;
    t_outlet* out_pred;
    t_float do_reject;
    t_atom value;
};

static void reject_if_float(t_reject_if* x, t_floatarg f)
{
    outlet_float(x->out_pred, f);

    if (x->do_reject == 0)
        outlet_float(x->x_obj.te_outlet, f);
}

static void reject_if_list(t_reject_if* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_list(x->out_pred, s, argc, argv);

    if (x->do_reject == 0)
        outlet_list(x->x_obj.te_outlet, s, argc, argv);
}

static void reject_if_any(t_reject_if* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_anything(x->out_pred, s, argc, argv);

    if (x->do_reject == 0)
        outlet_anything(x->x_obj.te_outlet, s, argc, argv);
}

static void reject_if_symbol(t_reject_if* x, t_symbol* s)
{
    outlet_symbol(x->out_pred, s);

    if (x->do_reject == 0)
        outlet_symbol(x->x_obj.te_outlet, s);
}

static void* reject_if_new()
{
    t_reject_if* x = reinterpret_cast<t_reject_if*>(pd_new(reject_if_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_pred = outlet_new(&x->x_obj, &s_anything);
    x->in_pred = floatinlet_new(&x->x_obj, &x->do_reject);
    x->do_reject = 0.0;
    return static_cast<void*>(x);
}

static void reject_if_free(t_reject_if* x)
{
    inlet_free(x->in_pred);
    outlet_free(x->out_pred);
}

extern "C" void setup_reject0x2eif()
{
    reject_if_class = class_new(gensym("reject.if"),
        reinterpret_cast<t_newmethod>(reject_if_new),
        reinterpret_cast<t_method>(reject_if_free),
        sizeof(t_reject_if), 0, A_NULL);
    class_addfloat(reject_if_class, reject_if_float);
    class_addlist(reject_if_class, reject_if_list);
    class_addsymbol(reject_if_class, reject_if_symbol);
    class_addanything(reject_if_class, reject_if_any);
}
