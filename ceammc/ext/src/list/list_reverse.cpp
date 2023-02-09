#include "list_reverse.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"
#include "ceammc_containers.h"

#include <algorithm>

ListReverse::ListReverse(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListReverse::onList(const AtomListView& lv)
{
    AtomList32 rev;
    rev.reserve(lv.size());
    rev.insert_back(lv);
    std::reverse(rev.begin(), rev.end());
    listTo(0, rev.view());
}

void ListReverse::onDataT(const MListAtom& ml)
{
    MListAtom res(ml);
    res.detachData();
    res->reverse();
    atomTo(0, res);
}

void setup_list_reverse()
{
    ObjectFactory<ListReverse> obj("list.reverse");
    obj.processData<DataTypeMList>();

    obj.setDescription("reverses the order of the elements in the list");
    obj.setCategory("list");
    obj.setKeywords({"list", "reverse"});
}
