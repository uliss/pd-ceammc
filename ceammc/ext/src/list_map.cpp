#include "ceammc.h"
#include "ceammc.hpp"
#include <m_pd.h>

t_class* list_map_class;
typedef struct list_map {
    t_object x_obj;
    t_inlet* map_in;
    t_outlet* map_out;
    ceammc::pd::atom_list* atoms;
    int values_left;
} t_list_map;

static void list_map_value(t_list_map* x, t_floatarg f)
{
    x->values_left--;
    t_atom v;
    SETFLOAT(&v, f);
    if (x->atoms != 0)
        x->atoms->push_back(v);
}

static void list_map_list(t_list_map* x, t_symbol* s, int argc, t_atom* argv)
{
    x->values_left = argc;

    // container for mapped values
    x->atoms = new ceammc::pd::atom_list;
    x->atoms->reserve(argc);

    // output value to map function
    // get mapped value in list_map_value()
    for (int i = 0; i < argc; i++) {
        outlet_float(x->map_out, atom_getfloat(&argv[i]));
    }

    // all values are mapped now
    if (x->values_left == 0)
        outlet_list(x->x_obj.te_outlet, &s_list, x->atoms->size(), x->atoms->data());

    delete x->atoms;
    x->atoms = 0;
}

static void* list_map_new()
{
    t_list_map* x = reinterpret_cast<t_list_map*>(pd_new(list_map_class));
    outlet_new(&x->x_obj, &s_list);
    x->map_out = outlet_new(&x->x_obj, &s_list);
    x->map_in = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("mapped value"));
    x->values_left = 0;
    x->atoms = 0;
    return (void*)x;
}

static void list_map_free(t_list_map* x)
{
    outlet_free(x->map_out);
    inlet_free(x->map_in);
}

extern "C" void setup_list0x2emap()
{
    list_map_class = class_new(gensym("list.map"),
        (t_newmethod)list_map_new, (t_method)list_map_free,
        sizeof(t_list_map), 0, A_NULL);
    class_addlist(list_map_class, list_map_list);
    class_addmethod(list_map_class, (t_method)list_map_value, gensym("mapped value"), A_DEFFLOAT, 0);
}
