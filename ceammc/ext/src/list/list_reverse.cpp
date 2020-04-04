#include "list_reverse.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

#include <algorithm>

ListReverse::ListReverse(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListReverse::onList(const AtomList& l)
{
    AtomList rev(l);
    rev.reverse();
    listTo(0, rev);
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
}
