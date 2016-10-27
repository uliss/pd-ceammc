#include "ceammc.h"
#include "ceammc.hpp"
#include <m_pd.h>

t_class* list_each_class;
typedef struct list_each {
    t_object x_obj;
    t_inlet* map_in;
    t_outlet* map_out;
    ceammc::pd::atom_list* atoms;
} t_list_each;

static void list_each_value(t_list_each* x, t_symbol* s, int argc, t_atom* argv)
{
    for (int i = 0; i < argc; i++) {
        if (x->atoms != 0)
            x->atoms->push_back(argv[i]);
    }
}

static void list_each_list(t_list_each* x, t_symbol* s, int argc, t_atom* argv)
{
    // container for mapped values
    x->atoms = new ceammc::pd::atom_list;
    x->atoms->reserve(argc);

    // output value to map function
    // get mapped value in list_each_value()
    for (int i = 0; i < argc; i++) {
        t_atom* a = argv + i;
        switch (a->a_type) {
        case A_FLOAT:
            outlet_float(x->map_out, atom_getfloat(a));
            break;
        case A_SYMBOL:
            outlet_symbol(x->map_out, atom_getsymbol(a));
            break;
        default:
            break;
        }
    }

    ceammc::pd::output(x->x_obj.te_outlet, *x->atoms);

    delete x->atoms;
    x->atoms = 0;
}

static void* list_each_new()
{
    t_list_each* x = reinterpret_cast<t_list_each*>(pd_new(list_each_class));
    outlet_new(&x->x_obj, &s_list);
    x->map_out = outlet_new(&x->x_obj, &s_list);
    x->map_in = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("mapped value"));
    x->atoms = 0;
    return static_cast<void*>(x);
}

static void list_each_free(t_list_each* x)
{
    outlet_free(x->map_out);
    inlet_free(x->map_in);
}

extern "C" void setup_list0x2eeach()
{
    list_each_class = class_new(gensym("list.each"),
        (t_newmethod)list_each_new, (t_method)list_each_free,
        sizeof(t_list_each), 0, A_NULL);
    class_addlist(list_each_class, list_each_list);
    class_addmethod(list_each_class, (t_method)list_each_value, gensym("mapped value"), A_GIMME, 0);
}
