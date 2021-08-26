#include "prop_set.h"
#include "ceammc.h"
#include "ceammc_factory.h"
#include "datatype_property.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

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

void PropSet::parseProperties() { }

void PropSet::onInlet(size_t n, const AtomListView& lv)
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

        if (dest->te_g.g_pd == canvas_class)
            processCanvasProps(reinterpret_cast<t_glist*>(dest), props_[n], lv);
        else
            processObjectProps(dest, props_[n], lv);
    }
}

const char* PropSet::annotateInlet(size_t n) const
{
    if (n < props_.size())
        return props_[n]->s_name;
    else
        return nullptr;
}

void PropSet::processCanvasProps(t_glist* dest, t_symbol* s, const AtomListView& lv)
{
    for (size_t i = 0; i < props_.size(); i++) {
        auto* full = PropertyStorage::makeFullName(props_[i], dest);
        PropertyPtr pp(full);
        if (pp)
            pp->setFromPdArgs(lv);
        else
            OBJ_ERR << "can't find property: " << s->s_name;
    }
}

void PropSet::processObjectProps(t_object* dest, t_symbol* s, const AtomListView& lv)
{
    auto fn = ceammc_get_propset_fn(dest);
    if (!fn)
        OBJ_ERR << "can't find property: " << s->s_name;
    else if (!fn(dest, s, lv.size(), lv.toPdData()))
        OBJ_ERR << "can't set property: " << s->s_name;
}

void setup_prop_set()
{
    ObjectFactory<PropSet> obj("prop.set", OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.addAlias("p.set");
    obj.addOutletInfo("connect to target objects");
}
