#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListEqual : public BaseObject {
    AtomList pattern_list_;

public:
    ListEqual(const PdArgs& a)
        : BaseObject(a)
        , pattern_list_(a.args)
    {
        createInlet();
        createOutlet();

        createProperty(new PointerProperty<AtomList>("@pattern", &pattern_list_, true));
    }

    void onInlet(size_t n, const AtomList& lst)
    {
        if (n != 1)
            return;

        pattern_list_ = lst;
    }

    void onList(const AtomList& lst)
    {
        floatTo(0, lst == pattern_list_ ? 1 : 0);
    }
};

extern "C" void setup_list0x2eequal()
{
    ObjectFactory<ListEqual>("list.equal");
}
