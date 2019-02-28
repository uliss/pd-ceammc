#include "ceammc_atomlist.h"
#include <m_pd.h>
#include <map>
#include <string>
#include <vector>

#define MSG_PREFIX "[prop<-] "

using namespace ceammc;
typedef std::map<t_symbol*, t_inlet*> InletIndexMap;

static t_class* prop_set_class;
struct t_prop {
    t_object x_obj;
    InletIndexMap* prop_map;
};

static void prop_set_dump(t_prop* x)
{
    InletIndexMap::iterator it;
    for (it = x->prop_map->begin(); it != x->prop_map->end(); ++it)
        post(MSG_PREFIX "dump: property %s", it->first->s_name);
}

static inline void add_prop_map(t_prop* x, t_symbol* s)
{
    t_inlet* in = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, s);
    (*x->prop_map)[s] = in;
}

static void pass_any(t_prop* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_anything(x->x_obj.te_outlet, s, argc, argv);
}

static void* prop_set_new(t_symbol*, int argc, t_atom* argv)
{
    t_prop* x = reinterpret_cast<t_prop*>(pd_new(prop_set_class));
    outlet_new(&x->x_obj, &s_anything);
    x->prop_map = new InletIndexMap;

    // use only symbol started from '@'
    AtomList args = AtomList(argc, argv).filtered(isProperty);
    for (size_t i = 0; i < args.size(); i++)
        add_prop_map(x, args.at(i).asSymbol());

    return static_cast<void*>(x);
}

static void prop_set_free(t_prop* x)
{
    delete x->prop_map;
}

extern "C" void setup_prop0x2eset()
{
    prop_set_class = class_new(gensym("prop.set"),
        reinterpret_cast<t_newmethod>(prop_set_new),
        reinterpret_cast<t_method>(prop_set_free),
        sizeof(t_prop), 0, A_GIMME, A_NULL);
    class_addcreator(reinterpret_cast<t_newmethod>(prop_set_new), gensym("prop<-"), A_GIMME, A_NULL);
    class_addanything(prop_set_class, pass_any);
    class_addmethod(prop_set_class, reinterpret_cast<t_method>(prop_set_dump), gensym("dump"), A_NULL);
    class_sethelpsymbol(prop_set_class, gensym("prop.set"));
}
