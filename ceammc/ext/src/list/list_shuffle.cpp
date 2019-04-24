#include "list_shuffle.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

#include <algorithm>

ListShuffle::ListShuffle(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListShuffle::onList(const AtomList& lst)
{
    AtomList res(lst);
    std::random_shuffle(res.begin(), res.end());
    listTo(0, res);
}

void ListShuffle::onDataT(const DataTPtr<DataTypeMList>& lst)
{
    DataTypeMList res(*lst);
    std::random_shuffle(res.begin(), res.end());
    dataTo(0, DataTPtr<DataTypeMList>(res));
}

void setup_list_shuffle()
{
    ObjectFactory<ListShuffle> obj("list.shuffle");
    obj.processData<DataTypeMList>();
}
