#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class MathNegate : public BaseObject {
public:
    MathNegate(const PdArgs& args)
        : BaseObject(args)
    {
        createOutlet();
    }

    void onFloat(t_float f) final
    {
        floatTo(0, -f);
    }

    void onList(const AtomList& lst) final
    {
        listTo(0, lst.mapFloat([](t_float f) { return -f; }));
    }
};

void setup_math_neg()
{
    ObjectFactory<MathNegate> obj("math.neg");
    obj.addAlias("-x");
}
