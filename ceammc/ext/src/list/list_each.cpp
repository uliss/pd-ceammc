#include "list_each.h"
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
    addProperty(step_prop_);

    // if we have positional step argument without @step
    const AtomList& args = positionalArguments();
    if (checkArgs(args, ARG_INT)) {
        int v = args[0].asInt();
        if (v > 1)
            step_prop_->setValue(v);
    }
}

void ListEach::onList(const AtomList& l)
{
    mapped_list_.clear();

    size_t step = step_prop_->value();

    // output single values
    if (step == 1) {
        for (size_t i = 0; i < l.size(); i += step)
            atomTo(1, l[i]);
    } else { // output as sublist
        for (size_t i = 0; i < l.size(); i += step)
            listTo(1, l.slice(i, i + step - 1));
    }

    return listTo(0, mapped_list_);
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
    mapped_list_.clear();
    size_t step = step_prop_->value();

    // output single values
    if (step == 1) {
        for (size_t i = 0; i < ml->size(); i += step)
            atomTo(1, ml->at(i));

    } else { // output as sublist
        AtomList l = ml->data();
        for (size_t i = 0; i < ml->size(); i += step)
            listTo(1, l.slice(i, i + step - 1));
    }

    atomTo(0, new DataTypeMList(mapped_list_));
}

void setup_list_each()
{
    ObjectFactory<ListEach> obj("list.each");
    obj.processData<DataTypeMList>();
}
