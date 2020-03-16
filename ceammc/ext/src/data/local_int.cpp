#include "local_int.h"

LocalInt::LocalInt(const PdArgs& a)
    : LocalIntBase(a)
{
    property("@value")->info().setType(PropValueType::INTEGER);
}

void setup_local_int()
{
    NumericIFaceFactory<LocalInt> obj("local.int");
}
