#include "local_int.h"

LocalInt::LocalInt(const PdArgs& a)
    : LocalIntBase(a)
{
}

void setup_local_int()
{
    NumericIFaceFactory<LocalInt> obj("local.int");

    obj.setDescription("canvas-scoped named integer variable");
    obj.setCategory("local");
    obj.setKeywords({"int", "local"});
}
