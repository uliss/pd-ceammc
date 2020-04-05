#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class MathReciprocal : public BaseObject {
public:
    MathReciprocal(const PdArgs& args)
        : BaseObject(args)
    {
        createOutlet();
    }

    void onFloat(t_float f) final
    {
        floatTo(0, f * f);
    }

    void onList(const AtomList& lst) final
    {
        listTo(0, lst.mapFloat([](t_float f) { return f * f; }));
    }
};

void setup_math_reciprocal()
{
    ObjectFactory<MathReciprocal> obj("math.squared");
    obj.addAlias("^2");
}
