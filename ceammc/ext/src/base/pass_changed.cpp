#include "ceammc_message.h"
#include <cfloat>
#include <cmath>
#include <cstring>
#include <m_pd.h>

#define OBJ_NAME "pass.changed"
#define MSG_PREFIX "[" OBJ_NAME "] "

t_class* pass_changed_class;

typedef ceammc::Message Message;
struct t_pass_changed {
    t_object x_obj;
    Message* x_prev;
};

static void pass_changed_reset(t_pass_changed* x)
{
    *x->x_prev = Message();
}

static void pass_changed_set(t_pass_changed* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    if (argc == 1) {
        *x->x_prev = Message(argv[0]);
    } else {
        *x->x_prev = Message(argc, argv);
    }
}

static void pass_changed_anything(t_pass_changed* x, t_symbol* s, int argc, t_atom* argv)
{
    Message v(s, argc, argv);
    if (x->x_prev->isEqual(v))
        return;

    *x->x_prev = v;
    v.output(x->x_obj.te_outlet);
}

static void pass_changed_bang(t_pass_changed* x)
{
    x->x_prev->output(x->x_obj.te_outlet);
}

static void pass_changed_float(t_pass_changed* x, t_floatarg f)
{
    Message v(f);
    if (x->x_prev->isEqual(v))
        return;

    *x->x_prev = v;
    v.output(x->x_obj.te_outlet);
}

static void pass_changed_list(t_pass_changed* x, t_symbol* s, int argc, t_atom* argv)
{
    Message v(argc, argv);
    if (x->x_prev->isEqual(v))
        return;

    *x->x_prev = v;
    v.output(x->x_obj.te_outlet);
}

static void pass_changed_symbol(t_pass_changed* x, t_symbol* s)
{
    Message v(s);
    if (x->x_prev->isEqual(v))
        return;

    *x->x_prev = v;
    v.output(x->x_obj.te_outlet);
}

static void* pass_changed_new()
{
    t_pass_changed* x = reinterpret_cast<t_pass_changed*>(pd_new(pass_changed_class));
    outlet_new(&x->x_obj, &s_float);
    x->x_prev = new Message();
    return static_cast<void*>(x);
}

static void pass_changed_free(t_pass_changed* x)
{
    delete x->x_prev;
}

extern "C" void setup_pass0x2echanged()
{
    pass_changed_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(pass_changed_new),
        reinterpret_cast<t_method>(pass_changed_free),
        sizeof(t_pass_changed), 0, A_NULL);
    class_addanything(pass_changed_class, pass_changed_anything);
    class_addbang(pass_changed_class, pass_changed_bang);
    class_addfloat(pass_changed_class, pass_changed_float);
    class_addlist(pass_changed_class, pass_changed_list);
    class_addsymbol(pass_changed_class, pass_changed_symbol);
    class_addmethod(pass_changed_class,
        reinterpret_cast<t_method>(pass_changed_reset),
        gensym("reset"), A_NULL);
    class_addmethod(pass_changed_class,
        reinterpret_cast<t_method>(pass_changed_set),
        gensym("set"), A_GIMME, A_NULL);
}
