#include "ceammc.hpp"
#include <algorithm>
#include <m_pd.h>

typedef ceammc::pd::atom_list atom_list;

t_class* list_zip_class;
struct t_list_zip {
    t_object x_obj;
    t_inlet* in_l2;
    atom_list* l2;
};

static void set_list2(t_list_zip* x, t_symbol* s, int argc, t_atom* argv)
{
    x->l2->clear();
    for (int i = 0; i < argc; i++) {
        x->l2->push_back(argv[i]);
    }
}

static void list_zip_list(t_list_zip* x, t_symbol* s, int argc, t_atom* argv)
{
    atom_list zipped;
    int len = std::min<int>(argc, x->l2->size());

    for (int i = 0; i < len; i++) {
        zipped.push_back(argv[i]);
        zipped.push_back(x->l2->at(i));
    }

    ceammc::pd::output(x->x_obj.te_outlet, zipped);
}

static void* list_zip_new()
{
    t_list_zip* x = reinterpret_cast<t_list_zip*>(pd_new(list_zip_class));
    outlet_new(&x->x_obj, &s_float);
    x->in_l2 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("set list2"));
    x->l2 = new atom_list();
    return static_cast<void*>(x);
}

static void list_zip_free(t_list_zip* x)
{
    inlet_free(x->in_l2);
    delete x->l2;
}

extern "C" void setup_list0x2ezip()
{
    list_zip_class = class_new(gensym("list.zip"),
        reinterpret_cast<t_newmethod>(list_zip_new),
        reinterpret_cast<t_method>(list_zip_free),
        sizeof(t_list_zip), 0, A_NULL);
    class_addlist(list_zip_class, list_zip_list);
    class_addmethod(list_zip_class,
        reinterpret_cast<t_method>(set_list2),
        gensym("set list2"), A_GIMME, 0);
}
