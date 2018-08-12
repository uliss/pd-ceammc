#include "list_each.h"
#include "../data/datatype_mlist.h"

static bool is_valid_step(int v)
{
    return v > 0 && v < 1024;
}

ListEach::ListEach(const PdArgs& a)
    : BaseObject(a)
    , mapped_mlist_(new DataTypeMList)
    , mode_(MODE_NORMAL)
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

ListEach::~ListEach()
{
    delete mapped_mlist_;
}

void ListEach::onList(const AtomList& l)
{
    mode_ = MODE_NORMAL;
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

    listTo(0, mapped_list_);
}

void ListEach::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    if (l.empty())
        return;

    if (mode_ == MODE_NORMAL) {
        for (size_t i = 0; i < l.size(); i++)
            mapped_list_.append(l[i]);
    } else {
        for (size_t i = 0; i < l.size(); i++)
            mapped_mlist_->append(l[i]);
    }
}

void ListEach::onDataT(const DataTypeMList& lst)
{
    mode_ = MODE_MLIST;
    mapped_mlist_->clear();

    size_t step = step_prop_->value();

    // output single values
    if (step == 1) {
        for (size_t i = 0; i < lst.size(); i += step)
            atomTo(1, lst.at(i).toAtom());

    } else { // output as sublist
        AtomList l = lst.toList();
        for (size_t i = 0; i < lst.size(); i += step)
            listTo(1, l.slice(i, i + step - 1));
    }

    dataTo(0, DataPtr(mapped_mlist_->clone()));
}

void setup_list_each()
{
    ObjectFactory<ListEach> obj("list.each");
    obj.processData<DataTypeMList>();
}
