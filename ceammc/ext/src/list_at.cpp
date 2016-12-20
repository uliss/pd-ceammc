#include "ceammc.hpp"
#include <cassert>
#include <m_pd.h>
#include <stdlib.h>

#define OBJ_NAME "list.at"
#define MSG_PREFIX "[" OBJ_NAME "] "

t_class* list_at_class;
struct t_list_at {
    t_object x_obj;
    t_inlet* idx_inlet;
    t_float idx;
};

static bool is_valid_index(t_float v)
{
    return ceammc::math::is_integer(v);
}

static void list_at_list(t_list_at* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!is_valid_index(x->idx)) {
        pd_error(x, MSG_PREFIX "list index should be integer number: %f", x->idx);
        return;
    }

    // convert to int from float
    int idx = static_cast<int>(x->idx);
    if (abs(idx) >= argc) {
        pd_error(x, MSG_PREFIX "index %i is out of range. should be < %i or > %i", idx, argc, -argc);
        return;
    }

    // negative elements counted from end
    if (idx < 0)
        idx += argc;

    assert(idx >= 0);

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

    if (!is_valid_index(idx)) {
        pd_error(x, MSG_PREFIX "list index should be integer number: %f. setting to 0", idx);
        x->idx = 0;
    } else
        x->idx = idx;

    return static_cast<void*>(x);
}

extern "C" void setup_list0x2eat()
{
    list_at_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(list_at_new),
        reinterpret_cast<t_method>(list_at_free),
        sizeof(t_list_at), 0, A_DEFFLOAT, 0);
    class_addlist(list_at_class, list_at_list);
}
