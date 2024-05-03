#include "ceammc_containers.h"
#include "ceammc_object_info.h"
#include "ceammc_output.h"
#include "ceammc_symbols.h"

#include <algorithm>
#include <cassert>
#include <m_pd.h>
#include <string>
#include <vector>

#define MSG_PREFIX "[prop~>] "
constexpr const char* OBJ_NAME = "prop.route~";
constexpr const char* OBJ_OBSOLETE = "prop.get~";

using namespace ceammc;
typedef std::pair<t_symbol*, t_outlet*> outlet_pair;
typedef std::vector<outlet_pair> outlet_vec;

static t_class* prop_route_tilde_class;
struct t_prop_route_tilde {
    t_object x_obj;
    outlet_vec* outlet_list;
    t_outlet* all_prop;
    t_float f;
};

static void prop_route_tilde_dump(t_prop_route_tilde* x)
{
    if (x->outlet_list) {
        for (auto& p : *x->outlet_list)
            post(MSG_PREFIX "dump: property %s", p.first->s_name);
    }
}

static inline void add_prop_map(t_prop_route_tilde* x, t_symbol* s)
{
    x->outlet_list->push_back(std::make_pair(s, outlet_new(&x->x_obj, &s_list)));
}

static inline t_outlet* find_outlet(t_prop_route_tilde* x, t_symbol* sel)
{
    auto it = std::find_if(
        x->outlet_list->begin(),
        x->outlet_list->end(),
        [&sel](const outlet_vec::value_type& el) { return el.first == sel; });

    return it == x->outlet_list->end() ? 0 : it->second;
}

static inline void route_output(t_prop_route_tilde* x, t_symbol* s, const AtomListView& args)
{
    auto outlet = find_outlet(x, s);
    if (outlet == x->all_prop) // not matched
        outlet_anything(outlet, s, (int)args.size(), args.toPdData());
    else
        outletAtomList(outlet, args, true);
}

static bool is_property(t_symbol* s) { return s->s_name[0] == '@'; }
static bool is_property(t_atom* a) { return (a->a_type == A_SYMBOL || a->a_type == A_DEFSYM) && is_property(a->a_w.w_symbol); }

static void prop_route_tilde_anything(t_prop_route_tilde* x, t_symbol* s, int argc, t_atom* argv)
{
    // pass thru non-properties
    if (!is_property(s)) {
        outlet_anything(x->x_obj.te_outlet, s, argc, argv);
        return;
    }

    // create list with selector
    AtomList32 args;
    args.push_back(s);
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    enum PropState {
        PROP_INIT,
        PROP_SEL,
        PROP_VAL
    };

    PropState state = PROP_INIT;
    size_t prop_idx = 0;
    const size_t LAST_IDX = args.size() - 1;

    for (size_t i = 0; i < args.size(); i++) {
        auto& a = args[i];
        t_symbol* last_prop = args[prop_idx].asSymbol();

        if (a.isProperty()) {
            if (state == PROP_INIT) { // init -> @prop
                state = PROP_SEL;
                prop_idx = 0;
            } else if (state == PROP_SEL) { // @prop -> @prop
                route_output(x, last_prop, {});
                prop_idx = i;
            } else { // value -> @new_prop
                route_output(x, last_prop, args.view().subView(prop_idx + 1, i - prop_idx - 1));
                prop_idx = i;
            }

            if (i == LAST_IDX) { // output last
                route_output(x, args[i].asSymbol(), {});
            }
        } else { // ? -> value
            state = PROP_VAL;
            if (i == LAST_IDX)
                route_output(x, last_prop, args.view().subView(prop_idx + 1, i - prop_idx));
        }
    }
}

static void* prop_route_tilde_new(t_symbol* name, int argc, t_atom* argv)
{
    if (name == gensym(OBJ_OBSOLETE))
        pd_error(nullptr, MSG_PREFIX "using obsolete object name: [%s], "
                                     "use the new name: [%s], the obsolete alias will be removed in the next ceammc releas",
            OBJ_OBSOLETE, OBJ_NAME);

    auto x = reinterpret_cast<t_prop_route_tilde*>(pd_new(prop_route_tilde_class));

    // main signal
    outlet_new(&x->x_obj, &s_signal);
    x->outlet_list = new outlet_vec;

    for (int i = 0; i < argc; i++) {
        if (!is_property(&argv[i]))
            continue;

        add_prop_map(x, atom_getsymbol(&argv[i]));
    }

    // last outlet
    x->all_prop = outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

static void prop_route_tilde_free(t_prop_route_tilde* x)
{
    delete x->outlet_list;

    if (x->all_prop)
        outlet_free(x->all_prop);
}

static t_int* prop_dsp_perform(t_int* w)
{
    auto in = reinterpret_cast<t_sample*>(w[1]);
    auto out = reinterpret_cast<t_sample*>(w[2]);
    auto n = w[3];

    while (n-- > 0)
        *out++ = (*in++);

    return (w + 4);
}

static void prop_setup_dsp(t_prop_route_tilde* /*x*/, t_signal** sp)
{
    dsp_add(prop_dsp_perform, 3, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void setup_prop_route_tilde()
{
    prop_route_tilde_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(prop_route_tilde_new),
        reinterpret_cast<t_method>(prop_route_tilde_free),
        sizeof(t_prop_route_tilde), 0, A_GIMME, A_NULL);

    ceammc::ObjectInfoStorage::instance().addAlias("prop~>", prop_route_tilde_class, reinterpret_cast<t_newmethod>(prop_route_tilde_new));
    ceammc::ObjectInfoStorage::instance().addAlias("@~>", prop_route_tilde_class, reinterpret_cast<t_newmethod>(prop_route_tilde_new));
    ceammc::ObjectInfoStorage::instance().addAlias(OBJ_OBSOLETE, prop_route_tilde_class, reinterpret_cast<t_newmethod>(prop_route_tilde_new));

    class_addanything(prop_route_tilde_class, reinterpret_cast<t_method>(prop_route_tilde_anything));

    class_addmethod(prop_route_tilde_class, reinterpret_cast<t_method>(prop_setup_dsp), sym::methods::sym_dsp(), A_CANT, A_NULL);
    class_addmethod(prop_route_tilde_class, reinterpret_cast<t_method>(prop_route_tilde_dump), sym::methods::sym_dump(), A_NULL);

    class_domainsignalin(prop_route_tilde_class, offsetof(t_prop_route_tilde, f));
    class_sethelpsymbol(prop_route_tilde_class, gensym(OBJ_NAME));
}
