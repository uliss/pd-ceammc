#include "ceammc.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_containers.h"
#include "ceammc_log.h"
#include "ceammc_object_info.h"
#include "ceammc_output.h"
#include "ceammc_symbols.h"

#include <map>

#define MSG_PREFIX "[prop->] "
constexpr const char* OBJ_NAME = "prop.route";
constexpr const char* OBJ_OBSOLETE = "prop.split";

using namespace ceammc;

struct outlet_info {
    t_outlet* outlet;
    t_symbol* name;
};

using outlet_vec = std::vector<outlet_info>;

static t_class* prop_route_class;
struct t_prop_route {
    t_object x_obj;
    outlet_vec* prop_list;
    t_outlet* all_prop;
};

static inline t_outlet* find_outlet(t_prop_route* x, t_symbol* sel)
{
    for (auto& o : *x->prop_list) {
        if (o.name == sel)
            return o.outlet;
    }

    return x->all_prop;
}

static inline void prop_route_output(t_prop_route* x, t_symbol* s, const AtomListView& args)
{
    auto outlet = find_outlet(x, s);
    if (outlet == x->all_prop) // not matched
        outlet_anything(outlet, s, (int)args.size(), args.toPdData());
    else
        outletAtomList(outlet, args, true);
}

static void prop_route_dump(t_prop_route* x)
{
    for (auto& x : *x->prop_list)
        post(MSG_PREFIX "dump: property %s", x.name->s_name);
}

static void prop_split_anything(t_prop_route* x, t_symbol* s, int argc, t_atom* argv)
{
    // pass thru non-properties
    if (s->s_name[0] != '@') {
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
                prop_route_output(x, last_prop, AtomListView());
                prop_idx = i;
            } else { // value -> @new_prop
                prop_route_output(x, last_prop, args.view().subView(prop_idx + 1, i - prop_idx - 1));
                prop_idx = i;
            }

            if (i == LAST_IDX) { // output last
                prop_route_output(x, args[i].asSymbol(), AtomListView());
            }
        } else { // ? -> value
            state = PROP_VAL;
            if (i == LAST_IDX)
                prop_route_output(x, last_prop, args.view().subView(prop_idx + 1, i - prop_idx));
        }
    }
}

static void* prop_route_new(t_symbol* name, int argc, t_atom* argv)
{
    if (name == gensym(OBJ_OBSOLETE))
        pd_error(nullptr, MSG_PREFIX "using obsolete object name: [%s], "
                                     "use the new name: [%s], the obsolete alias will be removed in the next ceammc release",
            OBJ_OBSOLETE, OBJ_NAME);

    auto x = reinterpret_cast<t_prop_route*>(pd_new(prop_route_class));
    outlet_new(&x->x_obj, &s_anything);
    x->prop_list = new outlet_vec;
    x->prop_list->reserve(argc);

    // use only symbol started from '@'
    for (auto& a : AtomListView(argv, argc)) {
        if (a.isProperty()) {
            auto o = outlet_new(&x->x_obj, &s_anything);
            x->prop_list->push_back({ o, a.asT<t_symbol*>() });
        } else
            LogPdObject(x, LOG_ERROR) << "property name expected (starting from '@'), got: " << a << ", skipping argument";
    }

    x->all_prop = outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

static void prop_split_free(t_prop_route* x)
{
    delete x->prop_list;
}

static const char* prop_route_annotate(t_prop_route* x, XletType type, int xlet_idx)
{
    if (type == XLET_IN) {
        if (xlet_idx == 0)
            return "any: input dataflow";
    } else {
        if (xlet_idx == 0)
            return "any: output dataflow without properties";
        else if (xlet_idx <= (int)x->prop_list->size())
            return x->prop_list->at(xlet_idx - 1).name->s_name;
        else
            return "unmatched properties";
    }

    return "";
}

void setup_prop_route()
{
    prop_route_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(prop_route_new),
        reinterpret_cast<t_method>(prop_split_free),
        sizeof(t_prop_route), 0, A_GIMME, A_NULL);

    ceammc::ObjectInfoStorage::instance().addAlias("prop->", prop_route_class, reinterpret_cast<t_newmethod>(prop_route_new));
    ceammc::ObjectInfoStorage::instance().addAlias("@->", prop_route_class, reinterpret_cast<t_newmethod>(prop_route_new));
    ceammc::ObjectInfoStorage::instance().addAlias(OBJ_OBSOLETE, prop_route_class, reinterpret_cast<t_newmethod>(prop_route_new));

    class_addanything(prop_route_class, reinterpret_cast<t_method>(prop_split_anything));
    class_addmethod(prop_route_class, reinterpret_cast<t_method>(prop_route_dump), sym::methods::sym_dump(), A_NULL);
    class_addmethod(prop_route_class, reinterpret_cast<t_method>(prop_route_annotate), SymbolTable::instance().s_annotate_fn, A_CANT, A_NULL);
    class_sethelpsymbol(prop_route_class, gensym(OBJ_NAME));
}
