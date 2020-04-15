#include "ceammc_factory.h"
#include "ceammc_object.h"
#include <cmath>

using namespace ceammc;

static t_float do_reciprocal(t_float v)
{
    return v == 0 ? NAN : (t_float(1.0) / v);
}

class MathReciprocal : public BaseObject {
public:
    MathReciprocal(const PdArgs& args)
        : BaseObject(args)
    {
        createOutlet();
    }

    void onFloat(t_float f) final
    {
        floatTo(0, do_reciprocal(f));
    }

    void onList(const AtomList& lst) final
    {
        listTo(0, lst.mapFloat(do_reciprocal));
    }
};

void setup_math_reciprocal()
{
    ObjectFactory<MathReciprocal> obj("math.reciprocal");
    obj.addAlias("1/x");
}
