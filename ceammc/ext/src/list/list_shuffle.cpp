#include "list_shuffle.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListShuffle::ListShuffle(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListShuffle::onList(const AtomList& lst)
{
    AtomList res(lst);
    res.shuffle();
    listTo(0, res);
}

void ListShuffle::onDataT(const MListAtom& ml)
{
    MListAtom res(*ml);
    res->shuffle();
    atomTo(0, res);
}

void setup_list_shuffle()
{
    ObjectFactory<ListShuffle> obj("list.shuffle");
    obj.processData<DataTypeMList>();
}
