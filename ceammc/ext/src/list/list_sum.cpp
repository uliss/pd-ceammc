#include "list_sum.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListSum::ListSum(const PdArgs& a)
    : ListBase(a)
{
    createOutlet();
}

void ListSum::onFloat(t_float f)
{
    floatTo(0, f);
}

void ListSum::onList(const AtomList& l)
{
    auto s = l.sum();
    if (s == boost::none)
        return;

    floatTo(0, *s);
}

void setup_list_sum()
{
    ObjectFactory<ListSum> obj("list.sum");
    obj.processData<DataTypeMList>();

    obj.setDescription("calculates sum of floats in list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "sum" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListSum::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListSum::setOutletsInfo(obj.classPointer(), { "float" });
}
