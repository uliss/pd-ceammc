#include "ceammc_globaldata.h"
#include <m_pd.h>

#define OBJ_NAME "global.int"
#define MSG_PREFIX "[" OBJ_NAME "] "

typedef ceammc::GlobalData<int> GlobalInt;

static t_class* global_int_class;
struct t_global_int {
    t_object x_obj;
    GlobalInt* x_global;
};

static void global_int_set(t_global_int* x, t_floatarg f)
{
    x->x_global->ref() = static_cast<int>(f);
}

static void global_int_output(t_global_int* x)
{
    outlet_float(x->x_obj.te_outlet, x->x_global->ref());
}

static void global_int_bang(t_global_int* x)
{
    global_int_output(x);
}

static void global_int_float(t_global_int* x, t_floatarg f)
{
    global_int_set(x, f);
    global_int_output(x);
}

static void* global_int_new(t_symbol* id)
{
    t_global_int* x = reinterpret_cast<t_global_int*>(pd_new(global_int_class));
    outlet_new(&x->x_obj, &s_float);
    x->x_global = new GlobalInt(id->s_name, OBJ_NAME);
    return static_cast<void*>(x);
}

static void global_int_free(t_global_int* x)
{
    delete x->x_global;
}

extern "C" void setup_global0x2eint()
{
    global_int_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(global_int_new),
        reinterpret_cast<t_method>(global_int_free),
        sizeof(t_global_int), 0, A_DEFSYMBOL, A_NULL);
    class_addbang(global_int_class, global_int_bang);
    class_addfloat(global_int_class, global_int_float);
    class_addmethod(global_int_class,
        reinterpret_cast<t_method>(global_int_set),
        gensym("set"), A_DEFFLOAT, A_NULL);
}
