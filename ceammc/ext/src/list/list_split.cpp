#include "list_split.h"
#include "ceammc_factory.h"

ListSplit::ListSplit(const ceammc::PdArgs& args)
    : BaseObject(args)
    , index_(0)
{
    index_ = new SizeTProperty("@at", positionalFloatArgument(0));
    createProperty(index_);

    createOutlet();
    createOutlet();
}

void ListSplit::onList(const AtomList& l)
{
    AtomList l1, l2;

    for (size_t i = 0; i < l.size(); i++) {
        if (i < index_->value())
            l1.append(l[i]);
        else
            l2.append(l[i]);
    }

    listTo(1, l2);
    listTo(0, l1);
}

extern "C" void setup_list0x2esplit()
{
    ObjectFactory<ListSplit> obj("list.split");
}
