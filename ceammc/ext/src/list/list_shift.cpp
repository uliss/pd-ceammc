#include "list_shift.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListShift::ListShift(const PdArgs& args)
    : BaseObject(args)
    , shift_(nullptr)
{
    createInlet();
    createOutlet();

    shift_ = new FloatProperty("@shift", 0);
    shift_->setArgIndex(0);
    addProperty(shift_);
}

void ListShift::onList(const AtomList& lst)
{
    listTo(0, list::shift(lst, shift_->value()));
}

void ListShift::onInlet(size_t n, const AtomListView& lst)
{
    shift_->set(lst);
}

void ListShift::onDataT(const MListAtom& ml)
{
    atomTo(0, MListAtom(list::shift(ml->data(), shift_->value())));
}

void setup_list_shift()
{
    ObjectFactory<ListShift> obj("list.shift");
    obj.processData<DataTypeMList>();
}
