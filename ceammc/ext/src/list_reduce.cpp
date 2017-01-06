#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListReduce : public BaseObject {
    AtomProperty* accum_prop_;
    Atom accum_;

public:
    ListReduce(const PdArgs& a)
        : BaseObject(a)
        , accum_prop_(0)
        , accum_(0.f)
    {
        createInlet();
        createOutlet();
        createOutlet();

        accum_prop_ = new AtomProperty("@accum", accum_);
        createProperty(accum_prop_);

        parseArguments();
        if (!args().empty()) {
            accum_prop_->setValue(args()[0]);
        }
    }

    void onList(const AtomList& l)
    {
        if (l.empty())
            return;

        accum_ = accum_prop_->value();

        for (size_t i = 0; i < l.size(); i++) {
            AtomList pair;
            pair.append(accum_);
            pair.append(l[i]);
            listTo(1, pair);
        }

        atomTo(0, accum_);
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n != 1)
            return;

        accum_= atomlistToValue<Atom>(l, Atom(0.f));
    }
};

extern "C" void setup_list0x2ereduce()
{
    ObjectFactory<ListReduce> obj("list.reduce");
}
