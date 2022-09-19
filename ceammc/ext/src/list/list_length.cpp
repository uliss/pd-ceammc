#include "list_length.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListLength::ListLength(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListLength::onList(const AtomListView& lv)
{
    floatTo(0, lv.size());
}

void ListLength::onDataT(const MListAtom& ml)
{
    floatTo(0, ml->size());
}

void setup_list_length()
{
    ObjectFactory<ListLength> obj("list.length");
    obj.addAlias("list.size");
    obj.processData<DataTypeMList>();

    obj.setDescription("output the number of atoms in the list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "length", "size" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListLength::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListLength::setOutletsInfo(obj.classPointer(), { "int" });
}
