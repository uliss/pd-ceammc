#include "replace.h"

using namespace ceammc;

Replace::Replace(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createInlet();
    createOutlet();

    createProperty(new PointerProperty<Atom>("@from", &from_, false));
    createProperty(new PointerProperty<Atom>("@to", &to_, false));

    parseArguments();

    if (from_.isNone() && args().size() > 0)
        from_ = args()[0];

    if (to_.isNone() && args().size() > 1)
        to_ = args()[1];
}

void Replace::onInlet(size_t n, const AtomList& l)
{
    if (l.empty())
        return;

    if (n == 1)
        from_ = l[0];
    else if (n == 2)
        to_ = l[0];
}

void Replace::onList(const AtomList& l)
{
    if (validateArgs()) {
        AtomList res(l);

        if (!to_.isNone())
            res.replaceAll(from_, to_);
        else
            res.removeAll(from_);

        listTo(0, res);
    } else {
        listTo(0, l);
    }
}

void Replace::onFloat(float v)
{
    if (validateArgs() && Atom(v) == from_)
        atomTo(0, to_);
    else
        floatTo(0, v);
}

void Replace::onSymbol(t_symbol* s)
{
    if (validateArgs() && Atom(s) == from_)
        atomTo(0, to_);
    else
        symbolTo(0, s);
}

bool Replace::validateArgs() const
{
    return !from_.isNone();
}

extern "C" void replace_setup()
{
    ObjectFactory<Replace> obj("replace");
}
