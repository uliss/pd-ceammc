#include "global_float.h"
#include "ceammc_factory.h"

GlobalFloat::GlobalFloat(const PdArgs& a)
    : GlobalFloatBase(a)
{
    createOutlet();
}

void setup_global_float()
{
    NumericIFaceFactory<GlobalFloat> obj("global.float");

    obj.setDescription("global named float object");
    obj.setCategory("global");
    obj.setKeywords({"float", "global"});
}
