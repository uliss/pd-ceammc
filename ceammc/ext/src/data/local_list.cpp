#include "local_list.h"
#include "ceammc_factory.h"
#include "data_protocol.h"
#include "datatype_mlist.h"

LocalList::LocalList(const PdArgs& a)
    : LocalListBase(a, "local.list")
{
    createCbListProperty(
        "@value",
        [this]() -> AtomList { return list(); },
        [this](const AtomList& l) -> bool { list() = l; return true; })
        ->setArgIndex(1);
}

void setup_local_list()
{
    ListIFaceFactory<LocalList> obj("local.list");
    obj.processData<DataTypeMList>();

    LocalList::registerMethods(obj);
}
