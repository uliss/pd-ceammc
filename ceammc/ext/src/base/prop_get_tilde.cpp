#include "ceammc_atomlist.h"
#include "ceammc_log.h"
#include "ceammc_object_info.h"

#include <algorithm>
#include <cassert>
#include <m_pd.h>
#include <string>
#include <vector>

#define MSG_PREFIX "[prop~>] "

using namespace ceammc;
typedef std::pair<t_symbol*, t_outlet*> OutletPair;
typedef std::vector<OutletPair> OutletList;

static t_class* prop_get_tilde_class;
struct t_prop_tilde {
    t_object x_obj;
    OutletList* outlet_list;
    t_outlet* all_prop;
    t_float f;
};

static void prop_get_dump(t_prop_tilde* x)
{
    for (auto& p : *x->outlet_list)
        post(MSG_PREFIX "dump: property %s", p.first->s_name);
}

static inline void add_prop_map(t_prop_tilde* x, t_symbol* s)
{
    x->outlet_list->push_back(std::make_pair(s, outlet_new(&x->x_obj, &s_list)));
}

static inline t_outlet* get_prop_tilde_outlet(t_prop_tilde* x, t_symbol* sel)
{
    auto it = std::find_if(
        x->outlet_list->begin(),
        x->outlet_list->end(),
        [&sel](const OutletList::value_type& el) { return el.first == sel; });

    return it == x->outlet_list->end() ? 0 : it->second;
}

static bool is_property(t_symbol* s) { return s->s_name[0] == '@'; }
static bool is_property(t_atom* a) { return (a->a_type == A_SYMBOL || a->a_type == A_DEFSYM) && is_property(a->a_w.w_symbol); }

static void prop_get_anything(t_prop_tilde* x, t_symbol* s, int argc, t_atom* argv)
{
    // pass thru non-properties
    if (!is_property(s)) {
        outlet_anything(x->x_obj.te_outlet, s, argc, argv);
        return;
    }

    t_symbol* prev_prop = s;
    int k = 0;
    for (int i = 0; i < argc; i++) {
        if (is_property(&argv[i])) {
            // outlet to prev property
            t_outlet* out = get_prop_tilde_outlet(x, prev_prop);

            // invariant
            assert(k <= i);

            // unknown property
            if (!out)
                outlet_anything(x->all_prop, prev_prop, i - k, argv + k);
            else
                outlet_anything(out, prev_prop, i - k, argv + k);

            // update property index
            k = i + 1;
            prev_prop = atom_getsymbol(&argv[i]);
        }
    }

    // invariant
    assert(k <= argc);

    // output last property
    t_outlet* out = get_prop_tilde_outlet(x, prev_prop);
    // unknown property
    if (!out)
        outlet_anything(x->all_prop, prev_prop, argc - k, argv + k);
    else
        outlet_anything(out, prev_prop, argc - k, argv + k);
}

static void* prop_get_new(t_symbol*, int argc, t_atom* argv)
{
    t_prop_tilde* x = reinterpret_cast<t_prop_tilde*>(pd_new(prop_get_tilde_class));

    // main signal
    outlet_new(&x->x_obj, &s_signal);
    x->outlet_list = new OutletList;

    for (int i = 0; i < argc; i++) {
        if (!is_property(&argv[i]))
            continue;

        add_prop_map(x, atom_getsymbol(&argv[i]));
    }

    // last outlet
    x->all_prop = outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

static void prop_get_free(t_prop_tilde* x)
{
    delete x->outlet_list;

    if (x->all_prop)
        outlet_free(x->all_prop);
}

static t_int* prop_dsp_perform(t_int* w)
{
    t_sample* in = reinterpret_cast<t_sample*>(w[1]);
    t_sample* out = reinterpret_cast<t_sample*>(w[2]);
    auto n = w[3];

    while (n-- > 0)
        *out++ = (*in++);

    return (w + 4);
}

static void prop_setup_dsp(t_prop_tilde* /*x*/, t_signal** sp)
{
    dsp_add(prop_dsp_perform, 3, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void setup_prop_get_tilde()
{
    prop_get_tilde_class = class_new(gensym("prop.get~"),
        reinterpret_cast<t_newmethod>(prop_get_new),
        reinterpret_cast<t_method>(prop_get_free),
        sizeof(t_prop_tilde), 0, A_GIMME, A_NULL);

    ceammc::ObjectInfoStorage::instance().addAlias("prop~>", prop_get_tilde_class, reinterpret_cast<t_newmethod>(prop_get_new));
    ceammc::ObjectInfoStorage::instance().addAlias("@~>", prop_get_tilde_class, reinterpret_cast<t_newmethod>(prop_get_new));

    class_addanything(prop_get_tilde_class, reinterpret_cast<t_method>(prop_get_anything));

    class_addmethod(prop_get_tilde_class, reinterpret_cast<t_method>(prop_setup_dsp), gensym("dsp"), A_CANT, A_NULL);
    class_addmethod(prop_get_tilde_class, reinterpret_cast<t_method>(prop_get_dump), gensym("dump"), A_NULL);

    class_domainsignalin(prop_get_tilde_class, offsetof(t_prop_tilde, f));
    class_sethelpsymbol(prop_get_tilde_class, gensym("prop.get~"));
}
