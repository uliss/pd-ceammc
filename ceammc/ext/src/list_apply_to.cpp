#include "ceammc.hpp"
#include "ceammc_atomlist.h"
#include <cstdlib>
#include <m_pd.h>

#define OBJ_NAME "list.apply_to"
#define MSG_PREFIX "[" OBJ_NAME "] "

using namespace ceammc;

static t_class* list_apply_to_class;
struct t_list_apply_to {
    t_object x_obj;
    int idx;
    AtomList* list;
    t_inlet* in1;
    t_outlet* out1;
};

static void list_mapped_value(t_list_apply_to* x, t_symbol*, int argc, t_atom* argv)
{
    // invalid mapped value
    if (argc != 1) {
        pd_error(x, MSG_PREFIX "invalid mapped value given");
        return;
    }

    if (x->list == 0) {
        pd_error(x, MSG_PREFIX "atom list not allocated");
        return;
    }

    Atom* old = x->list->relativeAt(x->idx);
    if (!old) {
        pd_error(x, MSG_PREFIX "invalid list index: %i", x->idx);
        return;
    }

    // setting new value
    *old = Atom(argv[0]);
}

static void list_apply_to_list(t_list_apply_to* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    x->list = new AtomList(static_cast<size_t>(argc), argv);
    // getting atom at relative position
    Atom* a = x->list->relativeAt(x->idx);

    // invalid position
    if (a == NULL) {
        pd_error(x, MSG_PREFIX "invalid list index: %i", x->idx);

        // pass unmodified if invalid index given
        x->list->output(x->x_obj.te_outlet);
        return;
    }

    // output to map function
    a->output(x->out1);

    // output mapped list to main outlet
    x->list->output(x->x_obj.te_outlet);

    delete x->list;
    x->list = 0;
}

static void* list_apply_to_new(t_floatarg idx)
{
    t_list_apply_to* x = reinterpret_cast<t_list_apply_to*>(pd_new(list_apply_to_class));
    outlet_new(&x->x_obj, &s_list);
    x->in1 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("mapped value"));
    x->out1 = outlet_new(&x->x_obj, &s_float);
    x->list = 0;

    if (!math::is_integer(idx)) {
        pd_error(x, MSG_PREFIX "list index should be integer. Using '0' as index position.");
        x->idx = 0;
    }

    x->idx = static_cast<int>(idx);
    return static_cast<void*>(x);
}

static void list_apply_to_free(t_list_apply_to* x)
{
    outlet_free(x->out1);
    inlet_free(x->in1);
    delete x->list;
}

extern "C" void setup_list0x2eapply_to()
{
    list_apply_to_class = class_new(gensym("list.apply_to"),
        reinterpret_cast<t_newmethod>(list_apply_to_new),
        reinterpret_cast<t_method>(list_apply_to_free),
        sizeof(t_list_apply_to),
        0,
        /* arguments: [list.apply_to LIST_INDEX] */
        A_DEFFLOAT,
        A_NULL);
    class_addlist(list_apply_to_class, list_apply_to_list);
    class_addmethod(list_apply_to_class,
        reinterpret_cast<t_method>(list_mapped_value),
        gensym("mapped value"),
        A_GIMME, 0);
}
