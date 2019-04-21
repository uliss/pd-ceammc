#include "list_reverse.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

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

void ListReverse::onDataT(const DataTPtr<DataTypeMList>& dptr)
{
    DataTypeMList res(*dptr);
    std::reverse(std::begin(res), std::end(res));
    dataTo(0, DataTPtr<DataTypeMList>(res));
}

void setup_list_reverse()
{
    ObjectFactory<ListReverse> obj("list.reverse");
    obj.processData<DataTypeMList>();
}
