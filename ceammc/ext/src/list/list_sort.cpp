#include "list_sort.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

#include <algorithm>

ListSort::ListSort(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListSort::onList(const AtomList& lst)
{
    // sort only floats and symbols
    auto pred = [](const Atom& a) { return a.isSymbol() || a.isFloat(); };

    AtomList res(lst.filter(pred));
    std::sort(res.begin(), res.end());
    listTo(0, res);
}

void ListSort::onDataT(const DataTPtr<DataTypeMList>& lst)
{
    DataTypeMList res(*lst);
    res.sort();
    dataTo(0, DataTPtr<DataTypeMList>(res));
}

void setup_list_sort()
{
    ObjectFactory<ListSort> obj("list.sort");
    obj.processData<DataTypeMList>();
}
