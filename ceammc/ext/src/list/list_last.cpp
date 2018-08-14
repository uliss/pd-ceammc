#include "list_last.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

ListLast::ListLast(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListLast::onList(const AtomList& lst)
{
    auto last = lst.last();

    if (last)
        atomTo(0, *last);
}

void ListLast::onDataT(const DataTypeMList& lst)
{
    if (lst.empty())
        return;

    atomTo(0, lst.at(lst.size() - 1).toAtom());
}

void setup_list_last()
{
    ObjectFactory<ListLast> obj("list.last");
    obj.processData<DataTypeMList>();
}
