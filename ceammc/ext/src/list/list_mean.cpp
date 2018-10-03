#include "list_mean.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListMean::ListMean(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListMean::onFloat(float f)
{
    floatTo(0, f);
}

void ListMean::onList(const AtomList& l)
{
    if (l.empty())
        return;

    auto avg = list::average(l);
    if (avg == boost::none)
        return;

    floatTo(0, *avg);
}

void ListMean::onDataT(const DataTypeMList& lst)
{
    onList(lst.toList());
}

void setup_list_mean()
{
    ObjectFactory<ListMean> obj("list.mean");
    obj.addAlias("list.average");

    obj.processData<DataTypeMList>();
}
