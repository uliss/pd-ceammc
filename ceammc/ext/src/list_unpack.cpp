#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListUnpack : public BaseObject {
public:
    ListUnpack(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        for (size_t i = 0; i < l.size(); i++)
            atomTo(0, l[i]);

        bangTo(1);
    }
};

extern "C" void setup_list0x2eunpack()
{
    ObjectFactory<ListUnpack> obj("list.unpack");
}
