#include "ceammc_globaldata.h"
#include <m_pd.h>

#define OBJ_NAME "global.float"
#define MSG_PREFIX "[" OBJ_NAME "] "

typedef ceammc::GlobalData<t_float> GlobalFloat;

static t_class* global_float_class;
struct t_global_float {
    t_object x_obj;
    GlobalFloat* x_global;
};

static void global_float_set(t_global_float* x, t_floatarg f)
{
    x->x_global->ref() = f;
}

static void global_float_output(t_global_float* x)
{
    outlet_float(x->x_obj.te_outlet, x->x_global->ref());
}

static void global_float_bang(t_global_float* x)
{
    global_float_output(x);
}

static void global_float_float(t_global_float* x, t_floatarg f)
{
    global_float_set(x, f);
    global_float_output(x);
}

static void* global_float_new(t_symbol* id)
{
    t_global_float* x = reinterpret_cast<t_global_float*>(pd_new(global_float_class));
    outlet_new(&x->x_obj, &s_float);
    x->x_global = new GlobalFloat(id->s_name, OBJ_NAME);
    return static_cast<void*>(x);
}

static void global_float_free(t_global_float* x)
{
    delete x->x_global;
}

extern "C" void setup_global0x2efloat()
{
    global_float_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(global_float_new),
        reinterpret_cast<t_method>(global_float_free),
        sizeof(t_global_float), 0, A_DEFSYMBOL, A_NULL);
    class_addbang(global_float_class, global_float_bang);
    class_addfloat(global_float_class, global_float_float);
    class_addmethod(global_float_class,
        reinterpret_cast<t_method>(global_float_set),
        gensym("set"), A_DEFFLOAT, A_NULL);
}
