#include "list_split.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

ListSplit::ListSplit(const ceammc::PdArgs& args)
    : BaseObject(args)
    , index_(nullptr)
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

void ListSplit::onDataT(const DataTypeMList& lst)
{
    DataTypeMList* l1 = new DataTypeMList;
    DataTypeMList* l2 = new DataTypeMList;

    for (size_t i = 0; i < lst.size(); i++) {
        if (i < index_->value())
            l1->append(lst[i]);
        else
            l2->append(lst[i]);
    }

    dataTo(1, DataPtr(l2));
    dataTo(0, DataPtr(l1));
}

void setup_list_split()
{
    ObjectFactory<ListSplit> obj("list.split");
    obj.processData<DataTypeMList>();
}
