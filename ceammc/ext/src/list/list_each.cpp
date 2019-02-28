#include "list_each.h"
#include "datatype_mlist.h"

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
    mode_ = l.anyOf(isData) ? MODE_DLIST : MODE_NORMAL;

    switch (mode_) {
    case MODE_DLIST:
        mapped_dlist_.clear();
        break;
    default:
        mapped_list_.clear();
    }

    size_t step = step_prop_->value();

    // output single values
    if (step == 1) {
        for (size_t i = 0; i < l.size(); i += step)
            atomTo(1, l[i]);
    } else { // output as sublist
        for (size_t i = 0; i < l.size(); i += step)
            listTo(1, l.slice(i, i + step - 1));
    }

    switch (mode_) {
    case MODE_DLIST:
        return listTo(0, mapped_dlist_.toList());
    default:
        return listTo(0, mapped_list_);
    }
}

void ListEach::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    if (l.empty())
        return;

    switch (mode_) {
    case MODE_NORMAL:
        mapped_list_.append(l);
        break;
    case MODE_DLIST:
        mapped_dlist_.append(l);
        break;
    default:
        mapped_mlist_->append(l);
        break;
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
