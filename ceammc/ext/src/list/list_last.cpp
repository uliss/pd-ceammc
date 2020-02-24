#include "list_last.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

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

void ListLast::onDataT(const DataTPtr<DataTypeMList>& dptr)
{
    if (dptr->empty())
        return;

    atomTo(0, dptr->at(dptr->size() - 1).asAtom());
}

void setup_list_last()
{
    ObjectFactory<ListLast> obj("list.last");
    obj.processData<DataTypeMList>();
}
