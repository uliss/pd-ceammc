#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class MathSquared : public BaseObject {
public:
    MathSquared(const PdArgs& args)
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

void setup_math_squared()
{
    ObjectFactory<MathSquared> obj("math.squared");
    obj.addAlias("^2");
}
