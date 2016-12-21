#include "ceammc_atomlist.h"
#include "ceammc_globaldata.h"
#include <m_pd.h>

#define OBJ_NAME "global.list"
#define MSG_PREFIX "[" OBJ_NAME "] "

typedef ceammc::GlobalData<ceammc::AtomList> GlobalList;

static t_class* global_list_class;
struct t_global_list {
    t_object x_obj;
    GlobalList* x_global;
};

static void global_list_output(t_global_list* x)
{
    ceammc::to_outlet(x->x_obj.te_outlet, x->x_global->ref());
}

static void global_list_set(t_global_list* x, t_symbol*, int argc, t_atom* argv)
{
    x->x_global->ref().fromPdData(static_cast<size_t>(argc), argv);
}

static void global_list_bang(t_global_list* x)
{
    global_list_output(x);
}

static void global_list_list(t_global_list* x, t_symbol* s, int argc, t_atom* argv)
{
    global_list_set(x, s, argc, argv);
    global_list_output(x);
}

static void* global_list_new(t_symbol* id)
{
    t_global_list* x = reinterpret_cast<t_global_list*>(pd_new(global_list_class));
    outlet_new(&x->x_obj, &s_float);
    x->x_global = new GlobalList(id->s_name, OBJ_NAME);
    return static_cast<void*>(x);
}

static void global_list_free(t_global_list* x)
{
    delete x->x_global;
}

extern "C" void setup_global0x2elist()
{
    global_list_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(global_list_new),
        reinterpret_cast<t_method>(global_list_free),
        sizeof(t_global_list), 0, A_DEFSYMBOL, A_NULL);
    class_addbang(global_list_class, global_list_bang);
    class_addlist(global_list_class, global_list_list);
    class_addmethod(global_list_class,
        reinterpret_cast<t_method>(global_list_set),
        gensym("set"), A_GIMME, A_NULL);
}
