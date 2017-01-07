#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListReduce : public BaseObject {
    Atom accum_;

public:
    ListReduce(const PdArgs& a)
        : BaseObject(a)
    {
        createInlet();
        createOutlet();
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        if (l.empty())
            return;

        accum_ = l[0];

        for (size_t i = 1; i < l.size(); i++) {
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

        accum_ = atomlistToValue<Atom>(l, Atom(0.f));
    }
};

extern "C" void setup_list0x2ereduce()
{
    ObjectFactory<ListReduce> obj("list.reduce");
    obj.mapFloatToList();
    obj.mapSymbolToList();
}
