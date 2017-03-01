#include "ceammc_factory.h"
#include "ceammc_object.h"
#include <cstdlib>

using namespace ceammc;

class ListChoice : public BaseObject {
public:
    ListChoice(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        size_t idx = static_cast<size_t>(rand()) % l.size();
        atomTo(0, l[idx]);
    }
};

extern "C" void setup_list0x2echoice()
{
    ObjectFactory<ListChoice> obj("list.choice");
    obj.mapFloatToList();
}
