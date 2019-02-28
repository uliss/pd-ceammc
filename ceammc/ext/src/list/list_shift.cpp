#include "list_shift.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListShift::ListShift(const PdArgs& args)
    : BaseObject(args)
    , shift_(nullptr)
{
    createInlet();
    createOutlet();

    shift_ = new FloatProperty("@shift", positionalFloatArgument(0, 0));
    createProperty(shift_);
}

void ListShift::onList(const AtomList& lst)
{
    listTo(0, list::shift(lst, shift_->value()));
}

void ListShift::onInlet(size_t n, const AtomList& lst)
{
    shift_->set(lst);
}

void ListShift::onDataT(const DataTypeMList& lst)
{
    auto is_float = [](const DataAtom& a) { return a.isAtom() && a.toAtom().isFloat(); };
    auto floats = lst.toList(is_float);

    DataTypeMList* res = new DataTypeMList(list::shift(floats, shift_->value()));
    dataTo(0, DataPtr(res));
}

void setup_list_shift()
{
    ObjectFactory<ListShift> obj("list.shift");
    obj.processData<DataTypeMList>();
}
