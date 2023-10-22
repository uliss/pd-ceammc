#include "local_float.h"
#include "ceammc_factory.h"

LocalFloat::LocalFloat(const PdArgs& a)
    : LocalFloatBase(a)
{
    createOutlet();
}

void setup_local_float()
{
    NumericIFaceFactory<LocalFloat> obj("local.float");

    obj.setDescription("canvas-scoped named float variable");
    obj.setCategory("local");
    obj.setKeywords({ "float", "local" });
}
