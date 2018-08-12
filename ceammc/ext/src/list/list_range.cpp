#include "list_range.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <iterator>

ListRange::ListRange(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListRange::onList(const AtomList& l)
{
    Atom min, max;
    if (l.range(min, max))
        listTo(0, AtomList(min, max));
}

void ListRange::onDataT(const DataTypeMList& l)
{
    auto is_atom = [](const DataAtom& a) { return a.isAtom(); };
    onList(l.toList(is_atom));
}

void setup_list_range()
{
    ObjectFactory<ListRange> obj("list.range");
    obj.addAlias("list.minmax");
    obj.processData<DataTypeMList>();
}
