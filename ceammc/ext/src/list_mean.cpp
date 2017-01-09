#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListMean : public BaseObject {
public:
    ListMean(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onFloat(float f)
    {
        floatTo(0, f);
    }

    void onList(const AtomList& l)
    {
        if (l.empty())
            return;

        floatTo(0, list::average(l));
    }
};

extern "C" void setup_list0x2emean()
{
    ObjectFactory<ListMean> obj("list.mean");
}
