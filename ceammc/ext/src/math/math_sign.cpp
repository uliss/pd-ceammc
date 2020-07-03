#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

static inline t_float do_sign(t_float v)
{
    if (v > 0.0)
        return 1;
    else if (v == 0.0)
        return 0;
    else
        return -1;
}

class MathSign : public BaseObject {
public:
    MathSign(const PdArgs& args)
        : BaseObject(args)
    {
        createOutlet();
    }

    void onFloat(t_float f) final
    {
        floatTo(0, do_sign(f));
    }

    void onList(const AtomList& lst) final
    {
        listTo(0, lst.mapFloat(do_sign));
    }
};

void setup_math_sign()
{
    ObjectFactory<MathSign> obj("math.sign");
}
