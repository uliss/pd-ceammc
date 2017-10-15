#include "ceammc_factory.h"
#include "ceammc_fn_vector.h"
#include "ceammc_object.h"

using namespace ceammc;

class VectorLength : public BaseObject {
public:
    VectorLength(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        floatTo(0, static_cast<float>(vector::magnitude(l)));
    }
};

extern "C" void setup_vector0x2elength()
{
    ObjectFactory<VectorLength> obj("vector.length");
    obj.mapFloatToList();
}
