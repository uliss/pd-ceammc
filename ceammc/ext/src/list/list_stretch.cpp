#include "list_stretch.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListStretch::ListStretch(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
{
    createInlet();
    createOutlet();

    n_ = new IntPropertyMinEq("@size", positionalFloatArgument(0, 1), 1);
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

void ListStretch::onDataT(const DataTypeMList& lst)
{
    if (lst.empty())
        return;

    dataTo(0, DataPtr(new DataTypeMList(list::stretch(lst.toList(), n_->value()))));
}

void setup_list_stretch()
{
    ObjectFactory<ListStretch> obj("list.stretch");
    obj.processData<DataTypeMList>();
}
