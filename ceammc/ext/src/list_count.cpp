#include "ceammc.hpp"
#include <algorithm>
#include <functional>
#include <m_pd.h>

t_class* list_count_class;
struct t_list_count {
    t_object x_obj;
    t_inlet* in_cmp;
    t_atom pattern;
    int init;
};

static void list_count_set_pattern(t_list_count* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!x->init || argc < 1)
        return;

    x->pattern = argv[0];
}

static void list_count_list(t_list_count* x, t_symbol* s, int argc, t_atom* argv)
{
    long n = std::count_if(argv, argv + argc, std::bind1st(ceammc::pd::AtomEq(), x->pattern));
    outlet_float(x->x_obj.te_outlet, n);
}

static void* list_count_new(t_symbol* s, int argc, t_atom* argv)
{
    t_list_count* x = reinterpret_cast<t_list_count*>(pd_new(list_count_class));
    outlet_new(&x->x_obj, &s_float);
    x->in_cmp = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("set pattern"));

    if (argc > 0) {
        x->pattern = argv[0];
        x->init = 1;
    } else {
        SETFLOAT(&x->pattern, 0);
        x->init = 0;
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
        gensym("set pattern"), A_GIMME, 0);
}
