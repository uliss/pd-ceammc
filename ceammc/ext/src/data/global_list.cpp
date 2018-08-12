#include "global_list.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

GlobalList::GlobalList(const PdArgs& a)
    : GlobalListBase(a)
{
}

void setup_global_list()
{
    ListIFaceFactory<GlobalList> obj("global.list");
    obj.processData<DataTypeMList>();
}
