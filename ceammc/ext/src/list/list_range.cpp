#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListRange : public BaseObject {
public:
    ListRange(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        if (l.empty())
            return;

        Atom min, max;
        if (l.range(min, max)) {
            AtomList res;
            res.append(min);
            res.append(max);
            listTo(0, res);
        }
    }
};

extern "C" void setup_list0x2erange()
{
    ObjectFactory<ListRange> obj("list.range");
}
