#include "ceammc_atomlist.h"
#include <m_pd.h>
#include <stdlib.h>

using namespace ceammc;

static t_class* list_delta_class;
struct t_list_delta {
    t_object x_obj;
    AtomList* stored_list;
    AtomList* out_list;
};

static void list_delta_clear(t_list_delta* x, t_symbol*, int, t_atom*)
{
    x->stored_list->clear();
    x->out_list->clear();
}

static void list_delta_bang(t_list_delta* x, t_symbol*, int, t_atom*)
{
    x->out_list->output(x->x_obj.te_outlet);
}

static void list_delta_list(t_list_delta* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    AtomList new_list(argc, argv);
    //    new_list.removeAll(notFloat); // remove all non float atoms
    AtomList delta_list(x->stored_list->sub(new_list, AtomList::PADZERO));

    delta_list.output(x->x_obj.te_outlet);
    *x->stored_list = new_list;
    *x->out_list = delta_list;
}

static void* list_delta_new()
{
    t_list_delta* x = reinterpret_cast<t_list_delta*>(pd_new(list_delta_class));
    outlet_new(&x->x_obj, &s_float);
    x->stored_list = new AtomList;
    x->out_list = new AtomList;
    return static_cast<void*>(x);
}

static void list_delta_free(t_list_delta* x)
{
    delete x->stored_list;
}

extern "C" void setup_list0x2edelta()
{
    list_delta_class = class_new(gensym("list.delta"),
        reinterpret_cast<t_newmethod>(list_delta_new),
        reinterpret_cast<t_method>(list_delta_free),
        sizeof(t_list_delta), 0, A_NULL);
    class_addlist(list_delta_class, list_delta_list);
    class_addanything(list_delta_class, list_delta_clear);
    class_addbang(list_delta_class, list_delta_bang);
}
