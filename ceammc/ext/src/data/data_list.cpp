#include "data_list.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

DataList::DataList(const PdArgs& a)
    : EditorListT<DataListIFace<BaseObject>>(a)
{
    createInlet();
    createOutlet();

    createCbListProperty(
        "@value",
        [this]() -> AtomList { return list_; },
        [this](const AtomListView& lv) -> bool { list_ = lv; return true; })
        ->setArgIndex(0);
}

void setup_data_list()
{
    ListIFaceFactory<DataList> obj("data.list");
    obj.processData<DataTypeMList>();

    DataList::registerMethods(obj);

    obj.setDescription("list container");
    obj.setCategory("data");
    obj.setKeywords({"data", "list"});
}
