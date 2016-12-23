#include "ceammc_atomlist.h"
#include <m_pd.h>

#define OBJ_NAME "@prop.get"
#define MSG_PREFIX "[" OBJ_NAME "] "

using namespace ceammc;

static t_class* prop_get_class;
struct t_prop {
    t_object x_obj;
    t_outlet* out1;
    t_symbol* prop;
};

static void prop_get_anything(t_prop* x, t_symbol* s, int argc, t_atom* argv)
{
    AtomList args(argc, argv);

    if (x->prop) {
        if (s == x->prop) {
            const size_t sz = args.size();

            if (sz < 1) {
                outlet_bang(x->out1);
                return;
            }

            if (sz == 1) {
                args.first()->output(x->out1);
                return;
            }

            args.output(x->out1);
            return;
        }
    } else {
        if (s->s_name[0] == '@') {
            args.output(x->out1);
            return;
        }
    }

    outlet_anything(x->x_obj.te_outlet, s, argc, argv);
}

static void* prop_get_new(t_symbol* s, int argc, t_atom* argv)
{
    t_prop* x = reinterpret_cast<t_prop*>(pd_new(prop_get_class));
    outlet_new(&x->x_obj, &s_anything);
    x->out1 = outlet_new(&x->x_obj, &s_anything);

    x->prop = 0;
    AtomList args(argc, argv);
    if (!args.empty())
        args.first()->getSymbol(&x->prop);

    return static_cast<void*>(x);
}

static void prop_get_free(t_prop* x)
{
    outlet_free(x->out1);
}

extern "C" void setup_0x40prop0x2eget()
{
    prop_get_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(prop_get_new),
        reinterpret_cast<t_method>(prop_get_free),
        sizeof(t_prop), 0, A_GIMME, A_NULL);
    class_addanything(prop_get_class, prop_get_anything);
}
