#include "ceammc_atomlist.h"
#include <m_pd.h>
#include <stdlib.h>

using namespace ceammc;

static t_class* list_integrator_class;
struct t_list_integrator {
    t_object x_obj;
    AtomList* stored_list;
};

static void list_integrator_clear(t_list_integrator* x, t_symbol*, int, t_atom*)
{
    x->stored_list->clear();
}

static void list_integrator_bang(t_list_integrator* x, t_symbol*, int, t_atom*)
{
    x->stored_list->output(x->x_obj.te_outlet);
}

static void list_integrator_list(t_list_integrator* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1)
        return;
    
    AtomList new_list(argc, argv);
    //    new_list.removeAll(notFloat); // remove all non float atoms
    AtomList integrator_list(x->stored_list->addTo(new_list, AtomList::PADZERO));
    
    integrator_list.output(x->x_obj.te_outlet);
    *x->stored_list = integrator_list;
}

static void* list_integrator_new()
{
    t_list_integrator* x = reinterpret_cast<t_list_integrator*>(pd_new(list_integrator_class));
    outlet_new(&x->x_obj, &s_float);
    x->stored_list = new AtomList;
    return static_cast<void*>(x);
}

static void list_integrator_free(t_list_integrator* x)
{
    delete x->stored_list;
}

extern "C" void setup_list0x2eintegrator()
{
    list_integrator_class = class_new(gensym("list.integrator"),
                                 reinterpret_cast<t_newmethod>(list_integrator_new),
                                 reinterpret_cast<t_method>(list_integrator_free),
                                 sizeof(t_list_integrator), 0, A_NULL);
    class_addlist(list_integrator_class, list_integrator_list);
    class_addanything(list_integrator_class, list_integrator_clear);
    class_addbang(list_integrator_class, list_integrator_bang);
}
