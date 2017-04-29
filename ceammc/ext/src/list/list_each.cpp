#include "list_each.h"

static bool is_valid_step(int v)
{
    return v > 0 && v < 1024;
}

extern "C" void setup_list0x2eeach()
{
    ObjectFactory<ListEach>("list.each");
}

ListEach::ListEach(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createOutlet();
    createOutlet();

    step_prop_ = new StepProperty("@step", 1, is_valid_step);
    createProperty(step_prop_);

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
            listTo(1, l.slice(i, i + step));
    }

    listTo(0, mapped_list_);
}

void ListEach::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    if (l.empty())
        return;

    for (size_t i = 0; i < l.size(); i++)
        mapped_list_.append(l[i]);
}
