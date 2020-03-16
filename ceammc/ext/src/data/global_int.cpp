#include "global_int.h"
#include "ceammc_factory.h"

GlobalInt::GlobalInt(const PdArgs& a)
    : GlobalIntBase(a)
{
    property("@value")->info().setType(PropValueType::INTEGER);
}

void setup_global_int()
{
    NumericIFaceFactory<GlobalInt> obj("global.int");
}
