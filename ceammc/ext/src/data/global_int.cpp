#include "global_int.h"

GlobalInt::GlobalInt(const PdArgs& a)
    : GlobalIntBase(a)
{
    createOutlet();
}

void setup_global_int()
{
    NumericIFaceFactory<GlobalInt> obj("global.int");

    obj.setDescription("global named integer object");
    obj.setCategory("global");
    obj.setKeywords({ "int", "global" });
}
