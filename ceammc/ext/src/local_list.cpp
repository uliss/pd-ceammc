#include "ceammc_atomlist.h"
#include "ceammc_globaldata.h"
#include <m_pd.h>
#include <string.h>

#define OBJ_NAME "local.list"
#define MSG_PREFIX "[" OBJ_NAME "] "

typedef ceammc::GlobalData<ceammc::AtomList> LocalList;

static t_class* local_list_class;
struct t_local_list {
    t_object x_obj;
    t_canvas *canvas;
    LocalList* x_global;
};

static void local_list_output(t_local_list* x)
{
    ceammc::to_outlet(x->x_obj.te_outlet, x->x_global->ref());
}

static void local_list_set(t_local_list* x, t_symbol*, int argc, t_atom* argv)
{
    x->x_global->ref().fromPdData(static_cast<size_t>(argc), argv);
}

static void local_list_bang(t_local_list* x)
{
    local_list_output(x);
}

static void local_list_list(t_local_list* x, t_symbol* s, int argc, t_atom* argv)
{
    local_list_set(x, s, argc, argv);
    local_list_output(x);
}

static void* local_list_new(t_symbol* id)
{
    t_local_list* x = reinterpret_cast<t_local_list*>(pd_new(local_list_class));
    x->canvas = canvas_getcurrent();
    
    //TODO fix
    char buf[16];
    sprintf(buf, "%lu", (long)x->canvas);
    
    std::string name = id->s_name;
    name += buf;
    
    outlet_new(&x->x_obj, &s_float);
    x->x_global = new LocalList(name.c_str(), OBJ_NAME);
    return static_cast<void*>(x);
}

static void local_list_free(t_local_list* x)
{
    delete x->x_global;
}

extern "C" void setup_local0x2elist()
{
    local_list_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(local_list_new),
        reinterpret_cast<t_method>(local_list_free),
        sizeof(t_local_list), 0, A_DEFSYMBOL, A_NULL);
    class_addbang(local_list_class, local_list_bang);
    class_addlist(local_list_class, local_list_list);
    class_addmethod(local_list_class,
        reinterpret_cast<t_method>(local_list_set),
        gensym("set"), A_GIMME, A_NULL);
}
