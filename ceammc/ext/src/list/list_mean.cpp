#include "list_mean.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListMean::ListMean(const PdArgs& a)
    : ListBase(a)
{
    createOutlet();
}

void ListMean::onFloat(t_float f)
{
    floatTo(0, f);
}

void ListMean::onList(const AtomListView& lv)
{
    if (lv.empty())
        return;

    auto avg = list::average(lv);
    if (avg == boost::none)
        return;

    floatTo(0, *avg);
}

void setup_list_mean()
{
    ObjectFactory<ListMean> obj("list.mean");
    obj.addAlias("list.average");

    obj.processData<DataTypeMList>();
}
