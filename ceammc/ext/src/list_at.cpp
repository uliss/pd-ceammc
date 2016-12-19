#include "ceammc.hpp"
#include <m_pd.h>

t_class* list_at_class;
struct t_list_at {
    t_object x_obj;
    t_inlet* idx_inlet;
    t_float idx;
};

static void list_at_list(t_list_at* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!ceammc::math::is_natural(x->idx)) {
        pd_error(x, "list index should be natural number: %f. setting to 0", x->idx);
        return;
    }

    int idx = static_cast<int>(x->idx);
    if (idx >= argc) {
        pd_error(x, "index %i is out of range. should be < %i", idx, argc);
        return;
    }

    outlet_list(x->x_obj.te_outlet, s, 1, argv + idx);
}

static void list_at_free(t_list_at* x)
{
    inlet_free(x->idx_inlet);
}

static void* list_at_new(t_floatarg idx)
{
    t_list_at* x = reinterpret_cast<t_list_at*>(pd_new(list_at_class));
    outlet_new(&x->x_obj, &s_float);
    x->idx_inlet = floatinlet_new(&x->x_obj, &x->idx);

    if (!ceammc::math::is_natural(idx)) {
        pd_error(x, "list index should be natural number: %f. setting to 0", idx);
        x->idx = 0;
    } else
        x->idx = idx;

    return static_cast<void*>(x);
}

extern "C" void setup_list0x2eat()
{
    list_at_class = class_new(gensym("list.at"),
        reinterpret_cast<t_newmethod>(list_at_new),
        reinterpret_cast<t_method>(list_at_free),
        sizeof(t_list_at), 0, A_DEFFLOAT, 0);
    class_addlist(list_at_class, list_at_list);
}
