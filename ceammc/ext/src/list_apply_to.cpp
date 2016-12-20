#include "ceammc.hpp"
#include <cstdlib>
#include <m_pd.h>

#define OBJ_NAME "list.apply_to"
#define MSG_PREFIX "[" OBJ_NAME "] "

t_class* list_apply_to_class;
struct t_list_apply_to {
    t_object x_obj;
    int x_apply_idx;
    t_atom mapped_value;
    ceammc::pd::atom_list* x_list;
    t_inlet* inlet1;
    t_outlet* outlet1;
};

static void list_mapped_value(t_list_apply_to* x, t_symbol* s, int argc, t_atom* argv)
{
    // invalid mapped value
    if (argc != 1) {
        pd_error(x, MSG_PREFIX "invalid mapped value given");
        return;
    }

    if (x->x_list == 0) {
        pd_error(x, MSG_PREFIX "atom list not allocated");
        return;
    }

    int idx = x->x_apply_idx;
    if (x->x_list->size() <= abs(idx)) {
        pd_error(x, MSG_PREFIX "invalid list index: %i", idx);
        return;
    }

    if(idx < 0)
        idx += x->x_list->size();

    x->x_list->at(idx) = argv[0];
}

static void list_apply_to_list(t_list_apply_to* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    int idx = x->x_apply_idx;
    if (abs(idx) >= argc) {
        // pass unmodified if invalid index given
        pd_error(x, MSG_PREFIX "index %i is out of range. should be < %i or > %i", idx, argc, -argc);
        outlet_list(x->x_obj.te_outlet, s, argc, argv);
        return;
    }

    // handle negative index
    if (idx < 0)
        idx += argc;

    x->x_list = new ceammc::pd::atom_list(argv, argv + argc);
    t_atom el = argv[idx];

    // output to right outlet to mapped objected
    if (ceammc::pd::is_float(el))
        outlet_float(x->outlet1, atom_getfloat(&el));
    else if (ceammc::pd::is_symbol(el))
        outlet_symbol(x->outlet1, atom_getsymbol(&el));
    else {
        pd_error(x, MSG_PREFIX "unsupported type: %i. skipping", el.a_type);
        return;
    }

    // output modified list to left outlet
    ceammc::pd::output(x->x_obj.te_outlet, *x->x_list);

    delete x->x_list;
    x->x_list = 0;
}

static void* list_apply_to_new(t_floatarg idx)
{
    t_list_apply_to* x = reinterpret_cast<t_list_apply_to*>(pd_new(list_apply_to_class));
    outlet_new(&x->x_obj, &s_list);
    x->inlet1 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("mapped value"));
    x->outlet1 = outlet_new(&x->x_obj, &s_float);
    x->x_list = 0;

    if (!ceammc::math::is_integer(idx)) {
        pd_error(x, MSG_PREFIX "list index should be integer. Using '0' as index position.");
        x->x_apply_idx = 0;
    }

    x->x_apply_idx = static_cast<int>(idx);
    return static_cast<void*>(x);
}

static void list_apply_to_free(t_list_apply_to* x)
{
    outlet_free(x->outlet1);
    inlet_free(x->inlet1);
    delete x->x_list;
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
