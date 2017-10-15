#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListSum : public BaseObject {
public:
    ListSum(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onFloat(float f) { floatTo(0, f); }
    void onList(const AtomList& l) { floatTo(0, l.sum()); }
};

extern "C" void setup_list0x2esum()
{
    ObjectFactory<ListSum>("list.sum");
}
