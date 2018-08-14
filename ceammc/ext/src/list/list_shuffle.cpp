#include "list_shuffle.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

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

void ListShuffle::onDataT(const DataTypeMList& lst)
{
    DataTypeMList* res = new DataTypeMList(lst);
    std::random_shuffle(res->begin(), res->end());
    dataTo(0, DataPtr(res));
}

void setup_list_shuffle()
{
    ObjectFactory<ListShuffle> obj("list.shuffle");
    obj.processData<DataTypeMList>();
}
