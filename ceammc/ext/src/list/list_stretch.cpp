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
    addProperty(n_);
}

void ListStretch::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    listTo(0, list::stretch(lst, n_->value()));
}

void ListStretch::onInlet(size_t n, const AtomListView& lst)
{
    n_->set(lst);
}

void ListStretch::onDataT(const MListAtom& ml)
{
    if (ml->empty())
        return;

    atomTo(0, MListAtom(list::stretch(ml->data(), n_->value())));
}

void setup_list_stretch()
{
    ObjectFactory<ListStretch> obj("list.stretch");
    obj.processData<DataTypeMList>();
}
