#include "ceammc_atomlist.h"
#include "ceammc_log.h"

#include <boost/unordered_map.hpp>
#include <m_pd.h>
#include <string>
#include <vector>

#define MSG_PREFIX "[prop~>] "

using namespace ceammc;
typedef boost::unordered_map<t_symbol*, t_outlet*> OutletIndexMap;

static t_class* prop_get_tilde_class;
struct t_prop_tilde {
    t_object x_obj;
    OutletIndexMap* prop_map;
    t_outlet* all_prop;
    t_float f;
};

static void prop_get_dump(t_prop_tilde* x)
{
    OutletIndexMap::iterator it;
    for (it = x->prop_map->begin(); it != x->prop_map->end(); ++it)
        post(MSG_PREFIX "dump: property %s", it->first->s_name);
}

static inline void add_prop_map(t_prop_tilde* x, t_symbol* s)
{
    t_outlet* out = outlet_new(&x->x_obj, &s_list);
    (*x->prop_map)[s] = out;
}

static inline t_outlet* get_prop_tilde_outlet(t_prop_tilde* x, t_symbol* sel)
{
    OutletIndexMap::iterator it = x->prop_map->find(sel);
    return it == x->prop_map->end() ? 0 : it->second;
}

static void prop_get_anything(t_prop_tilde* x, t_symbol* s, int argc, t_atom* argv)
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
        t_outlet* prop_out = get_prop_tilde_outlet(x, props[i].first()->asSymbol());
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
    t_prop_tilde* x = reinterpret_cast<t_prop_tilde*>(pd_new(prop_get_tilde_class));
    outlet_new(&x->x_obj, &s_signal);
    x->prop_map = new OutletIndexMap;

    // use only symbol started from '@'
    AtomList args = AtomList(argc, argv).filtered(isProperty);
    for (size_t i = 0; i < args.size(); i++)
        add_prop_map(x, args.at(i).asSymbol());

    x->all_prop = outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

static void prop_get_free(t_prop_tilde* x)
{
    delete x->prop_map;

    if (x->all_prop)
        outlet_free(x->all_prop);
}

static t_int* prop_dsp_perform(t_int* w)
{
    t_sample* in1 = (t_sample*)(w[1]);
    t_sample* out = (t_sample*)(w[2]);
    int n = (int)(w[3]);

    while (n-- > 0)
        *out++ = (*in1++);

    return (w + 4);
}

static void prop_setup_dsp(t_prop_tilde* x, t_signal** sp)
{
    dsp_add(prop_dsp_perform, 3, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

extern "C" void setup_prop0x2eget_tilde()
{
    prop_get_tilde_class = class_new(gensym("prop.get~"),
        reinterpret_cast<t_newmethod>(prop_get_new),
        reinterpret_cast<t_method>(prop_get_free),
        sizeof(t_prop_tilde), 0, A_GIMME, A_NULL);
    class_addcreator(reinterpret_cast<t_newmethod>(prop_get_new), gensym("prop~>"), A_GIMME, A_NULL);
    class_addanything(prop_get_tilde_class, prop_get_anything);

    class_addmethod(prop_get_tilde_class, reinterpret_cast<t_method>(prop_setup_dsp), gensym("dsp"), A_CANT);
    class_addmethod(prop_get_tilde_class, reinterpret_cast<t_method>(prop_get_dump), gensym("dump"), A_NULL);

    CLASS_MAINSIGNALIN(prop_get_tilde_class, t_prop_tilde, f);
    class_sethelpsymbol(prop_get_tilde_class, gensym("prop.get~"));
}
