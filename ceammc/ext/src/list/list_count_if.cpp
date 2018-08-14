#include "list_count_if.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

ListCountIf::ListCountIf(const PdArgs& a)
    : BaseObject(a)
    , count_(0)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListCountIf::onBang()
{
    floatTo(0, static_cast<float>(count_));
}

void ListCountIf::onList(const AtomList& lst)
{
    count_ = 0;
    lst.outputAtoms(outletAt(1));
    onBang();
}

void ListCountIf::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    size_t c = atomlistToValue<size_t>(l, 0);
    if (c > 0)
        count_++;
}

void ListCountIf::onDataT(const DataTypeMList& lst)
{
    onList(lst.toList());
}

void setup_list_count_if()
{
    ObjectFactory<ListCountIf> obj("list.count_if");
    obj.processData<DataTypeMList>();
}
