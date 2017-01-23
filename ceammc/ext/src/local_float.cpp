#include "ceammc_globaldata.h"
#include <m_pd.h>

#define OBJ_NAME "local.float"
#define MSG_PREFIX "[" OBJ_NAME "] "

typedef ceammc::GlobalData<t_float> GlobalFloat;

static t_class* local_float_class;
struct t_local_float {
    t_object x_obj;
    t_canvas *canvas;
    GlobalFloat* x_global;
};

static void local_float_set(t_local_float* x, t_floatarg f)
{
    x->x_global->ref() = f;
}

static void local_float_output(t_local_float* x)
{
    outlet_float(x->x_obj.te_outlet, x->x_global->ref());
}

static void local_float_bang(t_local_float* x)
{
    local_float_output(x);
}

static void local_float_float(t_local_float* x, t_floatarg f)
{
    local_float_set(x, f);
    local_float_output(x);
}

static void* local_float_new(t_symbol* id)
{
    t_local_float* x = reinterpret_cast<t_local_float*>(pd_new(local_float_class));
    outlet_new(&x->x_obj, &s_float);
    
    x->canvas = canvas_getcurrent();
    
    //TODO fix
    char buf[16];
    sprintf(buf, "%lu", (long)x->canvas);
    
    std::string name = id->s_name;
    name += buf;
    
    x->x_global = new GlobalFloat(name.c_str(), OBJ_NAME);
    return static_cast<void*>(x);
}

static void local_float_free(t_local_float* x)
{
    delete x->x_global;
}

extern "C" void setup_local0x2efloat()
{
    local_float_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(local_float_new),
        reinterpret_cast<t_method>(local_float_free),
        sizeof(t_local_float), 0, A_DEFSYMBOL, A_NULL);
    class_addbang(local_float_class, local_float_bang);
    class_addfloat(local_float_class, local_float_float);
    class_addmethod(local_float_class,
        reinterpret_cast<t_method>(local_float_set),
        gensym("set"), A_DEFFLOAT, A_NULL);
}
