#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListCount : public BaseObject {
    Atom pattern_;

public:
    ListCount(const PdArgs& a)
        : BaseObject(a)
    {
        createInlet();
        createOutlet();

        createCbProperty("@pattern", &ListCount::pattern);
        pattern_ = positionalArgument(0);
    }

    void onList(const AtomList& l)
    {
        floatTo(0, l.count(pattern_));
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n != 1)
            return;

        if (l.size() < 1)
            return;
        else if (l.size() == 1)
            pattern_ = l[0];
        else {
            OBJ_DBG << "search pattern can be only single element, not list. Using first element from " << l;
            pattern_ = l[0];
        }
    }

    AtomList pattern() const
    {
        return listFrom(pattern_);
    }
};

extern "C" void setup_list0x2ecount()
{
    ObjectFactory<ListCount> obj("list.count");
}
