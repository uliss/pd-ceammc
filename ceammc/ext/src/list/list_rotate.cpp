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

    step_ = new IntProperty("@step", 1);
    step_->setArgIndex(0);
    addProperty(step_);
}

void ListRotate::onList(const AtomListView& lv)
{
    listTo(0, list::rotate(lv, step_->value() * rotate_dir_));
}

void ListRotate::onInlet(size_t, const AtomListView& step)
{
    step_->set(step);
}

void ListRotate::onDataT(const MListAtom& ml)
{
    MListAtom res(*ml);
    res->rotateLeft(step_->value() * rotate_dir_);
    atomTo(0, res);
}

void setup_list_rotate()
{
    ObjectFactory<ListRotate> obj("list.rotate");
    obj.addAlias("list.<<");
    obj.addAlias("list.>>");
    obj.processData<DataTypeMList>();

    obj.setDescription("rotates list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "rotate" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListRotate::setInletsInfo(obj.classPointer(), { "list or Mlist", "int: rotation step" });
    ListRotate::setOutletsInfo(obj.classPointer(), { "list or Mlist" });
}
