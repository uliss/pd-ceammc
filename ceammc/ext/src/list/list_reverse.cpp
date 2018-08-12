#include "list_reverse.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

#include <algorithm>

ListReverse::ListReverse(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListReverse::onList(const AtomList& l)
{
    AtomList rev(l);
    rev.reverse();
    listTo(0, rev);
}

void ListReverse::onDataT(const DataTypeMList& lst)
{
    DataTypeMList* res = new DataTypeMList(lst);
    std::reverse(std::begin(*res), std::end(*res));
    dataTo(0, DataPtr(res));
}

void setup_list_reverse()
{
    ObjectFactory<ListReverse> obj("list.reverse");
    obj.processData<DataTypeMList>();
}
