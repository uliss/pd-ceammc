#include "ceammc_atomlist.h"
#include "ceammc_log.h"

#include <m_pd.h>
#include <map>
#include <string>
#include <vector>

#define MSG_PREFIX "[prop->] "

using namespace ceammc;
typedef std::map<t_symbol*, t_outlet*> OutletIndexMap;

static t_class* prop_get_class;
struct t_prop {
    t_object x_obj;
    OutletIndexMap* prop_map;
    t_outlet* all_prop;
};

static void prop_get_dump(t_prop* x)
{
    OutletIndexMap::iterator it;
    for (it = x->prop_map->begin(); it != x->prop_map->end(); ++it)
        post(MSG_PREFIX "dump: property %s", it->first->s_name);
}

static inline void add_prop_map(t_prop* x, t_symbol* s)
{
    t_outlet* out = outlet_new(&x->x_obj, &s_list);
    (*x->prop_map)[s] = out;
}

static inline t_outlet* get_prop_outlet(t_prop* x, t_symbol* sel)
{
    OutletIndexMap::iterator it = x->prop_map->find(sel);
    return it == x->prop_map->end() ? 0 : it->second;
}

static void prop_get_anything(t_prop* x, t_symbol* s, int argc, t_atom* argv)
{
    // pass thru non-properties
    if (s->s_name[0] != '@') {
        outlet_anything(x->x_obj.te_outlet, s, argc, argv);
        return;
    }

    AtomList args(argc, argv);
    args.insert(0, s);
    AtomList unmatched;

    std::deque<AtomList> props = args.properties();
    for (size_t i = 0; i < props.size(); i++) {
        // get mapped to property outlet
        t_outlet* prop_out = get_prop_outlet(x, props[i].first()->asSymbol());
        if (prop_out != 0) {
            props[i].slice(1).output(prop_out);
        } else
            unmatched.append(props[i]);
    }

    if (!unmatched.empty())
        unmatched.outputAsAny(x->all_prop);
}

static void* prop_get_new(t_symbol*, int argc, t_atom* argv)
{
    t_prop* x = reinterpret_cast<t_prop*>(pd_new(prop_get_class));
    outlet_new(&x->x_obj, &s_anything);
    x->prop_map = new OutletIndexMap;

    // use only symbol started from '@'
    AtomList args = AtomList(argc, argv).filtered(isProperty);
    for (size_t i = 0; i < args.size(); i++)
        add_prop_map(x, args.at(i).asSymbol());

    x->all_prop = outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

static void prop_get_free(t_prop* x)
{
    delete x->prop_map;

    if (x->all_prop)
        outlet_free(x->all_prop);
}

extern "C" void setup_prop0x2eget()
{
    prop_get_class = class_new(gensym("prop.get"),
        reinterpret_cast<t_newmethod>(prop_get_new),
        reinterpret_cast<t_method>(prop_get_free),
        sizeof(t_prop), 0, A_GIMME, A_NULL);
    class_addcreator(reinterpret_cast<t_newmethod>(prop_get_new), gensym("prop->"), A_GIMME, A_NULL);
    class_addanything(prop_get_class, prop_get_anything);
    class_addmethod(prop_get_class, reinterpret_cast<t_method>(prop_get_dump), gensym("dump"), A_NULL);
    class_sethelpsymbol(prop_get_class, gensym("prop.get"));
}
