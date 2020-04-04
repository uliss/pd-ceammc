#include "list_range.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

#include <iterator>

ListRange::ListRange(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListRange::onList(const AtomList& l)
{
    Atom min, max;
    if (l.filtered(
             [](const Atom& a) -> bool { return !a.isData(); })
            .range(min, max)) {
        listTo(0, AtomList(min, max));
    }
}

void ListRange::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_list_range()
{
    ObjectFactory<ListRange> obj("list.range");
    obj.addAlias("list.minmax");
    obj.processData<DataTypeMList>();
}
