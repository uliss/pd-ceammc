#include "list_rotate.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

static t_symbol* SYM_ROTATE_RIGHT = gensym("list.>>");

ListRotate::ListRotate(const PdArgs& a)
    : BaseObject(a)
    , step_(0)
    , rotate_dir_((a.creationName == SYM_ROTATE_RIGHT) ? ROTATE_RIGHT : ROTATE_LEFT)
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

void ListRotate::onDataT(const DataTypeMList& ml)
{
    DataPtr ptr(ml.rotateLeft(step_->value() * rotate_dir_).clone());
    dataTo(0, ptr);
}

void setup_list_rotate()
{
    ObjectFactory<ListRotate> obj("list.rotate");
    obj.addAlias("list.<<");
    obj.addAlias("list.>>");
    obj.processData<DataTypeMList>();
}
