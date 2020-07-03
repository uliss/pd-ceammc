#include "list_count_if.h"
#include "ceammc_factory.h"
#include "ceammc_output.h"
#include "datatype_mlist.h"

ListCountIf::ListCountIf(const PdArgs& a)
    : ListBase(a)
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

    auto out = outletAt(1);
    for(auto& a: lst)
        outletAtom(out, a);

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

void setup_list_count_if()
{
    ObjectFactory<ListCountIf> obj("list.count_if");
    obj.processData<DataTypeMList>();
}
