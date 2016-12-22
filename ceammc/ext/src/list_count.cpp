#include "ceammc.hpp"
#include "ceammc_atomlist.h"
#include <algorithm>
#include <functional>
#include <m_pd.h>

#define OBJ_NAME "list.сount"
#define MSG_PREFIX "[" OBJ_NAME "] "

using namespace ceammc;

static t_class* list_count_class;
struct t_list_count {
    t_object x_obj;
    t_inlet* in_cmp;
    Atom pattern;
};

static void list_count_set_pattern(t_list_count* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    if (argc > 1)
        pd_error(x, MSG_PREFIX "search pattern can be only single element, not list. Using first element.");

    x->pattern = Atom(argv[0]);
}

static void list_count_list(t_list_count* x, t_symbol*, int argc, t_atom* argv)
{
    size_t count = AtomList(static_cast<size_t>(argc), argv).count(x->pattern);
    outlet_float(x->x_obj.te_outlet, count);
}

static void* list_count_new(t_symbol*, int argc, t_atom* argv)
{
    t_list_count* x = reinterpret_cast<t_list_count*>(pd_new(list_count_class));
    outlet_new(&x->x_obj, &s_float);
    x->in_cmp = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("pattern"));

    // pattern specified
    if (argc > 0) {
        x->pattern = Atom(argv[0]);
    } else {
        pd_error(x, MSG_PREFIX "search pattern not specfied.");
        x->pattern = Atom();
    }

    return static_cast<void*>(x);
}

static void list_count_free(t_list_count* x)
{
    inlet_free(x->in_cmp);
}

extern "C" void setup_list0x2ecount()
{
    list_count_class = class_new(gensym("list.count"),
        reinterpret_cast<t_newmethod>(list_count_new),
        reinterpret_cast<t_method>(list_count_free),
        sizeof(t_list_count), 0, A_GIMME, A_NULL);
    class_addlist(list_count_class, list_count_list);
    class_addmethod(list_count_class,
        reinterpret_cast<t_method>(list_count_set_pattern),
        gensym("pattern"), A_GIMME, 0);
}
