#include "replace.h"
#include "ceammc_factory.h"

using namespace ceammc;

Replace::Replace(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createInlet();
    createOutlet();

    addProperty(new PointerProperty<Atom>("@from", &from_, PropValueAccess::READWRITE))
        ->setArgIndex(0);

    addProperty(new PointerProperty<Atom>("@to", &to_, PropValueAccess::READWRITE))
        ->setArgIndex(1);
}

void Replace::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        from_ = lv.empty() ? Atom() : lv[0];
    else if (n == 2)
        to_ = lv.empty() ? Atom() : lv[0];
}

void Replace::onAny(t_symbol* sel, const AtomListView& lv)
{
    if (validateArgs()) {
        AtomList res(sel);
        res.append(lv);
        if (!to_.isNone())
            res.replaceAll(from_, to_);
        else
            res.removeAll(from_);

        if (!res[0].isSymbol())
            res.insert(0, &s_list);

        anyTo(0, res);
    } else {
        anyTo(0, sel, lv);
    }
}

void Replace::onList(const AtomListView& lv)
{
    if (validateArgs()) {
        AtomList res(lv);

        if (!to_.isNone())
            res.replaceAll(from_, to_);
        else
            res.removeAll(from_);

        listTo(0, res.view());
    } else {
        listTo(0, lv);
    }
}

void Replace::onFloat(t_float v)
{
    if (validateArgs() && Atom(v) == from_) {
        if (!to_.isNone())
            atomTo(0, to_);
    } else
        floatTo(0, v);
}

void Replace::onSymbol(t_symbol* s)
{
    if (validateArgs() && Atom(s) == from_) {
        if (!to_.isNone())
            atomTo(0, to_);
    } else
        symbolTo(0, s);
}

bool Replace::validateArgs() const
{
    return !from_.isNone() && from_ != to_;
}

void setup_base_replace()
{
    ObjectFactory<Replace> obj("replace");

    obj.setDescription("Replace atoms in data stream");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "replace", "test" });
    obj.setCategory("flow");
    obj.setSinceVersion(0, 1);
}
