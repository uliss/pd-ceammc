#include "ceammc_factory.h"
#include "ceammc_fn_vector.h"
#include "ceammc_object.h"

using namespace ceammc;

class VectorNormalize : public BaseObject {
public:
    VectorNormalize(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onList(const AtomList& l)
    {
        AtomList res(l);
        double m = vector::magnitude(res);

        if (m == 0.0) {
            listTo(0, l);
            return;
        }

        res /= m;
        listTo(0, res);
    }
};

extern "C" void setup_vector0x2enormalize()
{
    ObjectFactory<VectorNormalize> obj("vector.normalize");
    obj.mapFloatToList();
}
