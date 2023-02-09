#include "global_list.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

GlobalList::GlobalList(const PdArgs& a)
    : EditorListT<GlobalListBase>(a)
{
    createCbListProperty(
        "@value",
        [this]() -> AtomList { return list(); },
        [this](const AtomListView& lv) -> bool { list() = lv; return true; })
        ->setArgIndex(1);
}

EditorTitleString GlobalList::editorTitle() const
{
    char buf[32];
    snprintf(buf, sizeof(buf) - 1, "GLOBAL.LIST (%s)", this->id()->s_name);
    return buf;
}

void setup_global_list()
{
    ListIFaceFactory<GlobalList> obj("global.list");
    obj.processData<DataTypeMList>();

    GlobalList::registerMethods(obj);

    obj.setDescription("global named list object");
    obj.setCategory("global");
    obj.setKeywords({"list", "global"});
}
