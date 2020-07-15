#include "list_split.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListSplit::ListSplit(const ceammc::PdArgs& args)
    : BaseObject(args)
    , index_(nullptr)
{
    index_ = new SizeTProperty("@at", 0);
    index_->setArgIndex(0);
    addProperty(index_);

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

void ListSplit::onDataT(const MListAtom& ml)
{
    MListAtom l1;
    MListAtom l2;

    for (size_t i = 0; i < ml->size(); i++) {
        if (i < index_->value())
            l1->append((*ml)[i]);
        else
            l2->append((*ml)[i]);
    }

    atomTo(1, l2);
    atomTo(0, l1);
}

void setup_list_split()
{
    ObjectFactory<ListSplit> obj("list.split");
    obj.processData<DataTypeMList>();

    obj.setDescription("splits list into two parts, the length of first list is specified by the argument");
    obj.addAuthor("Alex Nadzharov");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "split", "moses" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListSplit::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListSplit::setOutletsInfo(obj.classPointer(), { "list or Mlist: first", "list or Mlist: second" });
}
