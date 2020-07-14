#include "list_each.h"
#include "ceammc_output.h"
#include "datatype_mlist.h"

ListEach::ListEach(const PdArgs& a)
    : BaseObject(a)
    , mode_(MODE_NORMAL)
{
    createInlet();
    createOutlet();
    createOutlet();

    step_prop_ = new IntProperty("@step", 1);
    step_prop_->checkClosedRange(1, 1024);
    step_prop_->setArgIndex(0);
    addProperty(step_prop_);
}

void ListEach::onList(const AtomList& l)
{
    doEach(l);
    return listTo(0, mapped_list_);
}

void ListEach::doEach(const AtomList& l)
{
    mapped_list_.clear();

    size_t step = step_prop_->value();

    // output single values
    if (step == 1) {
        for (size_t i = 0; i < l.size(); i += step)
            atomTo(1, l[i]);
    } else { // output as sublist
        for (size_t i = 0; i < l.size(); i += step)
            listTo(1, l.view(i, step));
    }
}

void ListEach::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    if (l.empty())
        return;

    mapped_list_.append(l);
}

void ListEach::onDataT(const MListAtom& ml)
{
    doEach(ml->data());
    atomTo(0, MListAtom(mapped_list_));
}

void setup_list_each()
{
    ObjectFactory<ListEach> obj("list.each");
    obj.processData<DataTypeMList>();

    obj.setDescription("maps each list value via external side-chain");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "functional", "map", "iterate" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListEach::setInletsInfo(obj.classPointer(), { "list or Mlist", "value(s) from side-chain" });
    ListEach::setOutletsInfo(obj.classPointer(), { "list or Mlist", "atom or list to side-chain (depends on @step property)" });
}
