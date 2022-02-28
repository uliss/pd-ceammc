#include "global_list.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

GlobalList::GlobalList(const PdArgs& a)
    : EditorListT<GlobalListBase>(a, "global.list")
{
}

void setup_global_list()
{
    ListIFaceFactory<GlobalList> obj("global.list");
    obj.processData<DataTypeMList>();

    GlobalList::registerMethods(obj);
}
