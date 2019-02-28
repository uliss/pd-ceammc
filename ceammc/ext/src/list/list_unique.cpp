#include "list_unique.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListUnique::ListUnique(const ceammc::PdArgs& a)
    : BaseObject(a)
    , stable_(nullptr)
{
    stable_ = new BoolProperty("@stable", true);
    createProperty(stable_);

    createOutlet();
}

void ListUnique::onList(const AtomList& l)
{
    if (stable_->value())
        listTo(0, list::uniqueStable(l));
    else
        listTo(0, list::uniqueSorted(l));
}

void ListUnique::onDataT(const DataTypeMList& l)
{
    if (stable_->value())
        dataTo(0, DataPtr(new DataTypeMList(list::uniqueStable(l.toList()))));
    else
        dataTo(0, DataPtr(new DataTypeMList(list::uniqueSorted(l.toList()))));
}

void setup_list_unique()
{
    ObjectFactory<ListUnique> obj("list.unique");
    obj.processData<DataTypeMList>();
}
