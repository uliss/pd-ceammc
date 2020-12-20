#include "list_sort.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListSort::ListSort(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListSort::onList(const AtomList& lst)
{
    AtomList res(lst);
    res.sort();
    listTo(0, res);
}

void ListSort::onDataT(const MListAtom& ml)
{
    MListAtom res(*ml);
    res->sort();
    atomTo(0, res);
}

void setup_list_sort()
{
    ObjectFactory<ListSort> obj("list.sort");
    obj.processData<DataTypeMList>();
}
