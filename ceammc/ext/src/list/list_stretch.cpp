#include "list_stretch.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListStretch::ListStretch(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
{
    createInlet();
    createOutlet();

    n_ = new IntProperty("@size", 1);
    n_->setArgIndex(0);
    n_->checkPositive();
    createProperty(n_);
}

void ListStretch::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    listTo(0, list::stretch(lst, n_->value()));
}

void ListStretch::onInlet(size_t n, const AtomList& lst)
{
    n_->set(lst);
}

void ListStretch::onDataT(const DataTPtr<DataTypeMList>& dptr)
{
    if (dptr->empty())
        return;

    dataTo(0, DataTPtr<DataTypeMList>(DataTypeMList(list::stretch(dptr->toList(), n_->value()))));
}

void setup_list_stretch()
{
    ObjectFactory<ListStretch> obj("list.stretch");
    obj.processData<DataTypeMList>();
}
