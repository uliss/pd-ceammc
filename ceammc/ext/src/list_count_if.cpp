#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListCountIf : public BaseObject {
    size_t count_;

public:
    ListCountIf(const PdArgs& a)
        : BaseObject(a)
        , count_(0)
    {
        createInlet();
        createOutlet();
        createOutlet();
    }

    void onBang()
    {
        floatTo(0, static_cast<float>(count_));
    }

    void onList(const AtomList& lst)
    {
        count_ = 0;
        lst.outputAtoms(outletAt(1));
        onBang();
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n != 1)
            return;

        size_t c = atomlistToValue<size_t>(l, 0);
        if (c > 0)
            count_++;
    }
};

extern "C" void setup_list0x2ecount_if()
{
    ObjectFactory<ListCountIf>("list.count_if");
}
