#include "list_rotate.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListRotate::ListRotate(const PdArgs& a)
    : BaseObject(a)
    , step_(0)
{
    createInlet();
    createOutlet();

    step_ = new IntProperty("@step", int(positionalFloatArgument(0, 1)));
    createProperty(step_);
}

void ListRotate::onList(const AtomList& l)
{
    listTo(0, list::rotate(l, step_->value()));
}

void ListRotate::onInlet(size_t, const AtomList& step)
{
    step_->set(step);
}

extern "C" void setup_list0x2erotate()
{
    ObjectFactory<ListRotate> obj("list.rotate");
    obj.addAlias("list.<<");
}
