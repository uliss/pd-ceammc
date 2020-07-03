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

void ListLast::onDataT(const MListAtom& ml)
{
    if (ml->empty())
        return;

    atomTo(0, ml->at(ml->size() - 1));
}

void setup_list_last()
{
    ObjectFactory<ListLast> obj("list.last");
    obj.processData<DataTypeMList>();
}
