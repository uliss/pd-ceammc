#include "local_float.h"
#include "ceammc_factory.h"

LocalFloat::LocalFloat(const PdArgs& a)
    : LocalFloatBase(a)
{
    property("@value")->info().setType(PropertyInfoType::FLOAT);
}

void setup_local_float()
{
    NumericIFaceFactory<LocalFloat> obj("local.float");
}
