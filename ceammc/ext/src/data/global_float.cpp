#include "global_float.h"
#include "ceammc_factory.h"

GlobalFloat::GlobalFloat(const PdArgs& a)
    : GlobalFloatBase(a)
{
    property("@value")->info().setType(PropertyInfoType::FLOAT);
}

void setup_global_float()
{
    NumericIFaceFactory<GlobalFloat> obj("global.float");
}
