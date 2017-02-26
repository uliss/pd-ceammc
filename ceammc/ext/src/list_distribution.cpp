#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListDistribution : public BaseObject {
public:
    ListDistribution(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        listTo(0, list::countRepeats(l));
    }
};

extern "C" void setup_list0x2edistribution()
{
    ObjectFactory<ListDistribution> obj("list.distribution");
}
