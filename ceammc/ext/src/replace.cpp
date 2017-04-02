#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class Replace : public BaseObject {
    Atom from_;
    Atom to_;

public:
    Replace(const PdArgs& a)
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

    void onInlet(size_t n, const AtomList& l)
    {
        if (l.empty())
            return;

        if (n == 1)
            from_ = l[0];
        else if (n == 2)
            to_ = l[0];
    }

    void onList(const AtomList& l)
    {
        if (!validateArgs())
            return;

        AtomList res(l);

        if (!to_.isNone())
            res.replaceAll(from_, to_);
        else
            res.removeAll(from_);

        listTo(0, res);
    }

    void onFloat(float v)
    {
        if (!validateArgs())
            return;

        if (Atom(v) == from_)
            atomTo(0, to_);
        else
            floatTo(0, v);
    }

    void onSymbol(t_symbol* s)
    {
        if (!validateArgs())
            return;

        if (Atom(s) == from_)
            atomTo(0, to_);
        else
            symbolTo(0, s);
    }

    bool validateArgs() const
    {
        if (from_.isNone()) {
            OBJ_ERR << "replace subject is not specified...";
            return false;
        }

        return true;
    }
};

extern "C" void replace_setup()
{
    ObjectFactory<Replace> obj("replace");
}
