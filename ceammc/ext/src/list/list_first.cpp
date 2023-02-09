#include "list_first.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListFirst::ListFirst(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListFirst::onList(const AtomListView& lv)
{
    if (lv.empty())
        return;

    atomTo(0, lv[0]);
}

void ListFirst::onDataT(const MListAtom& ml)
{
    if (ml->empty())
        return;

    atomTo(0, ml->at(0));
}

void setup_list_first()
{
    ObjectFactory<ListFirst> obj("list.first");
    obj.processData<DataTypeMList>();

    obj.setDescription("returns first list element");
    obj.setCategory("list");
}
