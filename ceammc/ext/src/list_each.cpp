#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListEach : public BaseObject {
    AtomList mapped_list_;

public:
    ListEach(const PdArgs& a)
        : BaseObject(a)
    {
        createInlet();
        createOutlet();
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        mapped_list_.clear();

        for (size_t i = 0; i < l.size(); i++)
            atomTo(1, l[i]);

        listTo(0, mapped_list_);
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n != 1)
            return;

        if (l.empty())
            return;

        mapped_list_.append(l[0]);
    }
};

extern "C" void setup_list0x2eeach()
{
    ObjectFactory<ListEach>("list.each");
}
