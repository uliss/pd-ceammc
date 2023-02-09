#include "list_sort.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

#include <algorithm>

ListSort::ListSort(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListSort::onList(const AtomListView& lv)
{
    AtomList32 res;
    res.insert_back(lv);
    std::sort(res.begin(), res.end());
    listTo(0, res.view());
}

void ListSort::onDataT(const MListAtom& ml)
{
    MListAtom res(*ml);
    res->sort();
    atomTo(0, res);
}

void setup_list_sort()
{
    ObjectFactory<ListSort> obj("list.sort");
    obj.processData<DataTypeMList>();

    obj.setDescription("sorts list values by ascending order");
    obj.setCategory("list");
}
