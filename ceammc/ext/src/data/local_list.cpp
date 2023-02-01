#include "local_list.h"
#include "ceammc_factory.h"
#include "data_protocol.h"
#include "datatype_mlist.h"

LocalList::LocalList(const PdArgs& a)
    : LocalListBase(a)
{
    createCbListProperty(
        "@value",
        [this]() -> AtomList { return list(); },
        [this](const AtomListView& lv) -> bool { list() = lv; return true; })
        ->setArgIndex(1);
}

EditorTitleString LocalList::editorTitle() const
{
    char buf[32];
    snprintf(buf, sizeof(buf) - 1, "LOCAL.LIST (%s)", this->id()->s_name);
    return buf;
}

void setup_local_list()
{
    ListIFaceFactory<LocalList> obj("local.list");
    obj.processData<DataTypeMList>();

    LocalList::registerMethods(obj);
}
