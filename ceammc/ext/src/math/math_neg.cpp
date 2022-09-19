#include "ceammc_containers.h"
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

    void onList(const AtomListView& lv) final
    {
        SmallAtomList res;
        res.reserve(lv.size());
        for (auto& a : lv)
            res.push_back(a * -1);

        listTo(0, res.view());
    }
};

void setup_math_neg()
{
    ObjectFactory<MathNegate> obj("math.neg");
    obj.addAlias("-x");
}
