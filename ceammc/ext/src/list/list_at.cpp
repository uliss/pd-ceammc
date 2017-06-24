#include "list_at.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"

ListAt::ListAt(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createOutlet();

    pos_ = new ListProperty("@index", positionalArguments().filtered(isFloat));
    if (pos_->value().empty())
        pos_->set(AtomList(0.0));

    createProperty(pos_);

    at_method_ = new SymbolEnumProperty("@method", "rel");
    at_method_->appendEnum("clip");
    at_method_->appendEnum("wrap");
    at_method_->appendEnum("fold");
    createProperty(at_method_);

    createProperty(new SymbolEnumAlias("@rel", at_method_, gensym("rel")));
    createProperty(new SymbolEnumAlias("@clip", at_method_, gensym("clip")));
    createProperty(new SymbolEnumAlias("@wrap", at_method_, gensym("wrap")));
    createProperty(new SymbolEnumAlias("@fold", at_method_, gensym("fold")));
}

void ListAt::onInlet(size_t idx, const AtomList& l)
{
    if (idx != 1)
        return;

    if (!l.allOf(isFloat)) {
        OBJ_ERR << "invalid indexes given: " << l;
        return;
    }

    pos_->set(l);
}

void ListAt::onList(const AtomList& l)
{
    AtomList& p = pos_->value();

    // single item
    if (p.size() == 1) {
        const Atom* a = at(l, p[0]);
        if (!a)
            return;

        atomTo(0, *a);
        return;
    }

    // multiple items
    AtomList res;
    res.reserve(p.size());
    for (size_t i = 0; i < p.size(); i++) {
        const Atom* a = at(l, p[i].asInt(0));
        if (!a)
            continue;

        res.append(*a);
    }

    listTo(0, res);
}

const Atom* ListAt::at(const AtomList& l, const Atom& p)
{
    const Atom* a = 0;

    if (!p.isInteger()) {
        OBJ_ERR << "invalid index value: " << p;
        return 0;
    }

    int pos = p.asInt(0);

    if (at_method_->is("clip"))
        a = l.clipAt(pos);
    else if (at_method_->is("wrap"))
        a = l.wrapAt(pos);
    else if (at_method_->is("fold"))
        a = l.foldAt(pos);
    else
        a = l.relativeAt(pos);

    if (a == 0)
        OBJ_ERR << "invalid index value: " << p;

    return a;
}

extern "C" void setup_list0x2eat()
{
    ObjectFactory<ListAt> obj("list.at");
}
