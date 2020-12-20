#include "ceammc.h"
#include "ceammc_atomlist.h"
#include "ceammc_log.h"
#include "ceammc_output.h"

#include <map>

#define MSG_PREFIX "[prop->] "

using namespace ceammc;

struct OutletInfo {
    t_outlet* outlet;
    t_symbol* name;
};

using OutletList = std::vector<OutletInfo>;

static t_class* prop_split_class;
struct t_prop_split {
    t_object x_obj;
    OutletList* prop_list;
    t_outlet* all_prop;
};

static inline t_outlet* find_outlet(t_prop_split* x, t_symbol* sel)
{
    for (auto& o : *x->prop_list) {
        if (o.name == sel)
            return o.outlet;
    }

    return x->all_prop;
}

static inline void prop_split_output(t_prop_split* x, t_symbol* s, const AtomListView& args)
{
    auto outlet = find_outlet(x, s);
    if (outlet == x->all_prop) // not matched
        outlet_anything(outlet, s, (int)args.size(), args.toPdData());
    else
        outletAtomListView(outlet, args, true);
}

static void prop_split_dump(t_prop_split* x)
{
    for (auto& x : *x->prop_list)
        post(MSG_PREFIX "dump: property %s", x.name->s_name);
}

static void prop_split_anything(t_prop_split* x, t_symbol* s, int argc, t_atom* argv)
{
    // pass thru non-properties
    if (s->s_name[0] != '@') {
        outlet_anything(x->x_obj.te_outlet, s, argc, argv);
        return;
    }

    t_atom pd_list[argc + 1];
    SETSYMBOL(pd_list, s);
    for (int i = 0; i < argc; i++)
        pd_list[i + 1] = argv[i];

    AtomListView args(pd_list, argc + 1);

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
                prop_split_output(x, last_prop, AtomListView());
                prop_idx = i;
            } else { // value -> @new_prop
                prop_split_output(x, last_prop, args.subView(prop_idx + 1, i - prop_idx - 1));
                prop_idx = i;
            }

            if (i == LAST_IDX) { // output last
                prop_split_output(x, args[i].asSymbol(), AtomListView());
            }
        } else { // ? -> value
            state = PROP_VAL;
            if (i == LAST_IDX)
                prop_split_output(x, last_prop, args.subView(prop_idx + 1, i - prop_idx));
        }
    }
}

static void* prop_split_new(t_symbol*, int argc, t_atom* argv)
{
    t_prop_split* x = reinterpret_cast<t_prop_split*>(pd_new(prop_split_class));
    outlet_new(&x->x_obj, &s_anything);
    x->prop_list = new OutletList;
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

static void prop_split_free(t_prop_split* x)
{
    delete x->prop_list;
}

static const char* prop_split_annotate(t_prop_split* x, XletType type, int xlet_idx)
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

void setup_prop_split()
{
    prop_split_class = class_new(gensym("prop.split"),
        reinterpret_cast<t_newmethod>(prop_split_new),
        reinterpret_cast<t_method>(prop_split_free),
        sizeof(t_prop_split), 0, A_GIMME, A_NULL);
    class_addcreator(reinterpret_cast<t_newmethod>(prop_split_new), gensym("prop->"), A_GIMME, A_NULL);
    class_addcreator(reinterpret_cast<t_newmethod>(prop_split_new), gensym("@->"), A_GIMME, A_NULL);

    class_addanything(prop_split_class, reinterpret_cast<t_method>(prop_split_anything));
    class_addmethod(prop_split_class, reinterpret_cast<t_method>(prop_split_dump), gensym("dump"), A_NULL);
    class_addmethod(prop_split_class, reinterpret_cast<t_method>(prop_split_annotate), SymbolTable::instance().s_annotate_fn, A_CANT, A_NULL);
    class_sethelpsymbol(prop_split_class, gensym("prop.split"));
}
