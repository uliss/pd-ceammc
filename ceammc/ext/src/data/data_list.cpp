#include "data_list.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

DataList::DataList(const PdArgs& a)
    : EditorObject<DataListIFace<BaseObject>>(a, "DATA.LIST")
{
    createInlet();
    createOutlet();

    createCbListProperty(
        "@value",
        [this]() -> AtomList { return list_; },
        [this](const AtomList& l) -> bool { list_ = l; return true; })
        ->setArgIndex(0);


}

void DataList::editorAddLine(t_symbol* sel, const AtomListView& lv)
{
    for (auto& a : lv)
        list().append(a);
}

void DataList::editorClear()
{
    list().clear();
}

AtomListView DataList::getContentForEditor() const
{
    return list();
}

void setup_data_list()
{
    ListIFaceFactory<DataList> obj("data.list");
    obj.processData<DataTypeMList>();

    DataList::registerMethods(obj);
}
