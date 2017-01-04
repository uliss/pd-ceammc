#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListNormalize : public BaseObject {
public:
    ListNormalize(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onList(const AtomList& lst)
    {
        AtomList out(lst.filtered(isFloat));

        if (!out.normalizeFloats()) {
            OBJ_ERR << "Invalid list values: " << lst;
            return;
        }

        listTo(0, out);
    }
};

extern "C" void setup_list0x2enormalize()
{
    ObjectFactory<ListNormalize> obj("list.normalize");
}
