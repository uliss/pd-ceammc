#include "ceammc_atomlist.h"
#include <m_pd.h>

#define OBJ_NAME "list.count_if"
#define MSG_PREFIX "[" OBJ_NAME "] "

using namespace ceammc;

static t_class* list_count_if_class;
struct t_list_count_if {
    t_object x_obj;
    t_inlet* x_in1;
    t_outlet* x_out1;
    size_t x_count;
};

static void from_predicate(t_list_count_if* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    x->x_count += atom_getfloat(argv);
}

static void list_count_if_list(t_list_count_if* x, t_symbol* s, int argc, t_atom* argv)
{
    x->x_count = 0;
    ceammc::AtomList lst(argc, argv);
    lst.outputAtoms(x->x_out1);
    outlet_float(x->x_obj.te_outlet, x->x_count);
}

static void* list_count_if_new()
{
    t_list_count_if* x = reinterpret_cast<t_list_count_if*>(pd_new(list_count_if_class));
    outlet_new(&x->x_obj, &s_float);
    x->x_count = 0;
    x->x_in1 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("pass"));
    x->x_out1 = outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

static void list_count_if_free(t_list_count_if* x)
{
}

extern "C" void setup_list0x2ecount_if()
{
    list_count_if_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(list_count_if_new),
        reinterpret_cast<t_method>(list_count_if_free),
        sizeof(t_list_count_if), 0, A_NULL);
    class_addlist(list_count_if_class, list_count_if_list);
    class_addmethod(list_count_if_class,
        reinterpret_cast<t_method>(from_predicate),
        gensym("pass"), A_GIMME, 0);
}
