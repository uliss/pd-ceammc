#include "list_rotate.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListRotate::ListRotate(const PdArgs& a)
    : BaseObject(a)
    , step_(0)
    , rotate_dir_((a.creationName == gensym("list.>>")) ? ROTATE_RIGHT : ROTATE_LEFT)
{
    createInlet();
    createOutlet();

    step_ = new IntProperty("@step", int(positionalFloatArgument(0, 1)));
    createProperty(step_);
}

void ListRotate::onList(const AtomList& l)
{
    listTo(0, list::rotate(l, step_->value() * rotate_dir_));
}

void ListRotate::onInlet(size_t, const AtomList& step)
{
    step_->set(step);
}

void ListRotate::onDataT(const DataTPtr<DataTypeMList>& ml)
{
    dataTo(0, DataTPtr<DataTypeMList>(ml->rotateLeft(step_->value() * rotate_dir_)));
}

void setup_list_rotate()
{
    ObjectFactory<ListRotate> obj("list.rotate");
    obj.addAlias("list.<<");
    obj.addAlias("list.>>");
    obj.processData<DataTypeMList>();
}
