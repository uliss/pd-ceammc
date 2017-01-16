#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListReverse : public BaseObject {
public:
    ListReverse(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        AtomList rev(l);
        rev.reverse();
        listTo(0, rev);
    }
};

extern "C" void setup_list0x2ereverse()
{
    ObjectFactory<ListReverse> obj("list.reverse");
}
