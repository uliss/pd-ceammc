#include "ceammc_globaldata.h"
#include <m_pd.h>

#define OBJ_NAME "local.int"
#define MSG_PREFIX "[" OBJ_NAME "] "

typedef ceammc::GlobalData<int> GlobalInt;

static t_class* local_int_class;
struct t_local_int {
    t_object x_obj;
    t_canvas *canvas;
    GlobalInt* x_global;
};

static void local_int_set(t_local_int* x, t_floatarg f)
{
    x->x_global->ref() = static_cast<int>(f);
}

static void local_int_output(t_local_int* x)
{
    outlet_float(x->x_obj.te_outlet, x->x_global->ref());
}

static void local_int_bang(t_local_int* x)
{
    local_int_output(x);
}

static void local_int_float(t_local_int* x, t_floatarg f)
{
    local_int_set(x, f);
    local_int_output(x);
}

static void* local_int_new(t_symbol* id)
{
    t_local_int* x = reinterpret_cast<t_local_int*>(pd_new(local_int_class));
    outlet_new(&x->x_obj, &s_float);
    
    x->canvas = canvas_getcurrent();
    
    //TODO fix
    char buf[16];
    sprintf(buf, "%lu", (long)x->canvas);
    
    std::string name = id->s_name;
    name += buf;
    
    
    x->x_global = new GlobalInt(name.c_str(), OBJ_NAME);
    return static_cast<void*>(x);
}

static void local_int_free(t_local_int* x)
{
    delete x->x_global;
}

extern "C" void setup_local0x2eint()
{
    local_int_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(local_int_new),
        reinterpret_cast<t_method>(local_int_free),
        sizeof(t_local_int), 0, A_DEFSYMBOL, A_NULL);
    class_addbang(local_int_class, local_int_bang);
    class_addfloat(local_int_class, local_int_float);
    class_addmethod(local_int_class,
        reinterpret_cast<t_method>(local_int_set),
        gensym("set"), A_DEFFLOAT, A_NULL);
}
