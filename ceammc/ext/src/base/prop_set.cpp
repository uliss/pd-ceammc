#include "prop_set.h"
#include "ceammc.h"
#include "ceammc_factory.h"

extern "C" {
#include "m_imp.h"
}

using SetPropertyFn = int (*)(t_object*, t_symbol*, int, t_atom*);

PropSet::PropSet(const PdArgs& args)
    : BaseObject(args)
{
    for (auto& a : args.args) {
        if (a.isProperty()) {
            props_.push_back(a.asSymbol());
            createInlet();
        } else
            OBJ_ERR << "property name expected (starting from '@'), got: " << a << ", skipping argument";
    }

    createOutlet();
}

void PropSet::parseProperties() {}

void PropSet::onInlet(size_t n, const AtomList& lst)
{
    // inlet index correction
    if (n-- == 0)
        return;

    t_outlet* outlet = nullptr;
    auto conn = obj_starttraverseoutlet(owner(), &outlet, 0);
    while (conn) {
        t_object* dest;
        t_inlet* inletp;
        int whichp;
        conn = obj_nexttraverseoutlet(conn, &dest, &inletp, &whichp);
        SetPropertyFn fn = reinterpret_cast<SetPropertyFn>(
            zgetfn(&dest->te_g.g_pd, SymbolTable::instance().s_propset_fn));

        if (!fn) {
            OBJ_ERR << "can't find properties: " << props_[n];
            continue;
        } else
            fn(dest, props_[n], lst.size(), lst.toPdData());
    }
}

const char* PropSet::annotateInlet(size_t n) const
{
    if (n < props_.size())
        return props_[n]->s_name;
    else
        return nullptr;
}

void setup_prop_set()
{
    ObjectFactory<PropSet> obj("prop.set", OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.addAlias("p.set");
    obj.addOutletInfo("connect to target objects");
}
