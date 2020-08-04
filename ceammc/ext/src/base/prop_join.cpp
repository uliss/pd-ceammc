#include "ceammc.h"
#include "ceammc_atomlist.h"
#include "ceammc_log.h"

#include <vector>

#define MSG_PREFIX "[prop<-] "

using namespace ceammc;
typedef std::vector<t_symbol*> PropList;

static t_class* prop_join_class;
struct t_prop_join {
    t_object x_obj;
    PropList* prop_list;
};

static inline void add_prop_list(t_prop_join* x, t_symbol* s)
{
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, s);
    x->prop_list->push_back(s);
}

static void* prop_join_new(t_symbol*, int argc, t_atom* argv)
{
    t_prop_join* x = reinterpret_cast<t_prop_join*>(pd_new(prop_join_class));
    outlet_new(&x->x_obj, &s_anything);

    x->prop_list = new PropList;
    x->prop_list->reserve(argc);

    // use only symbol started from '@'
    for (auto& a : AtomListView(argv, argc)) {
        if (a.isProperty())
            add_prop_list(x, a.asSymbol());
        else
            LogPdObject(x, LOG_ERROR) << "property name expected (starting from '@'), got: " << a << ", skipping argument";
    }

    return static_cast<void*>(x);
}

static void prop_join_free(t_prop_join* x)
{
    delete x->prop_list;
}

static void prop_join_any(t_prop_join* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_anything(x->x_obj.te_outlet, s, argc, argv);
}

static void prop_join_dump(t_prop_join* x)
{
    for (auto& p : *x->prop_list)
        post(MSG_PREFIX "dump: property %s", p->s_name);
}

void setup_prop_join()
{
    prop_join_class = class_new(gensym("prop.join"),
        reinterpret_cast<t_newmethod>(prop_join_new),
        reinterpret_cast<t_method>(prop_join_free),
        sizeof(t_prop_join), 0, A_GIMME, A_NULL);
    class_addcreator(reinterpret_cast<t_newmethod>(prop_join_new), gensym("prop<-"), A_GIMME, A_NULL);
    class_addanything(prop_join_class, reinterpret_cast<t_method>(prop_join_any));
    class_addmethod(prop_join_class, reinterpret_cast<t_method>(prop_join_dump), SymbolTable::instance().s_dump_fn, A_NULL);
    class_sethelpsymbol(prop_join_class, gensym("prop.join"));
}
