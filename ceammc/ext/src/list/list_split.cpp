#include "list_split.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

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

void ListSplit::onDataT(const DataTPtr<DataTypeMList>& dptr)
{
    DataTypeMList l1;
    DataTypeMList l2;

    for (size_t i = 0; i < dptr->size(); i++) {
        if (i < index_->value())
            l1.append((*dptr)[i]);
        else
            l2.append((*dptr)[i]);
    }

    dataTo(1, DataTPtr<DataTypeMList>(l2));
    dataTo(0, DataTPtr<DataTypeMList>(l1));
}

void setup_list_split()
{
    ObjectFactory<ListSplit> obj("list.split");
    obj.processData<DataTypeMList>();
}
