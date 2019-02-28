#include "list_sum.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"

ListSum::ListSum(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListSum::onFloat(float f)
{
    floatTo(0, f);
}

void ListSum::onList(const AtomList& l)
{
    auto s = l.sum();
    if (s == boost::none)
        return;

    floatTo(0, *s);
}

void ListSum::onDataT(const DataTypeMList& l)
{
    onList(l.toList());
}

void setup_list_sum()
{
    ObjectFactory<ListSum> obj("list.sum");
    obj.processData<DataTypeMList>();
}
