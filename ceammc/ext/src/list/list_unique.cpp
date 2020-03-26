#include "list_unique.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListUnique::ListUnique(const ceammc::PdArgs& a)
    : BaseObject(a)
    , stable_(nullptr)
{
    stable_ = new BoolProperty("@stable", true);
    addProperty(stable_);

    createOutlet();
}

void ListUnique::onList(const AtomList& l)
{
    if (stable_->value())
        listTo(0, list::uniqueStable(l));
    else
        listTo(0, list::uniqueSorted(l));
}

void ListUnique::onDataT(const DataTPtr<DataTypeMList>& l)
{
    if (stable_->value())
        dataTo(0, DataTPtr<DataTypeMList>(DataTypeMList(list::uniqueStable(l->toList()))));
    else
        dataTo(0, DataTPtr<DataTypeMList>(DataTypeMList(list::uniqueSorted(l->toList()))));
}

void setup_list_unique()
{
    ObjectFactory<ListUnique> obj("list.unique");
    obj.processData<DataTypeMList>();
}
