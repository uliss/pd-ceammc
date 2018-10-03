#include "list_first.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"

ListFirst::ListFirst(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListFirst::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    atomTo(0, lst[0]);
}

void ListFirst::onDataT(const DataTypeMList& lst)
{
    if (lst.empty())
        return;

    atomTo(0, lst.at(0).toAtom());
}

void setup_list_first()
{
    ObjectFactory<ListFirst> obj("list.first");
    obj.processData<DataTypeMList>();
}
