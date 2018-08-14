#include "list_distribution.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListDistribution::ListDistribution(const PdArgs& a)
    : BaseObject(a)
    , normalize_(nullptr)
{
    createOutlet();

    normalize_ = new BoolProperty("@normalize", true);
    createProperty(normalize_);
}

void ListDistribution::onList(const AtomList& l)
{
    listTo(0, list::countRepeats(l, normalize_->value()));
}

void ListDistribution::onDataT(const DataTypeMList& lst)
{
    onList(lst.toList());
}

void setup_list_distribution()
{
    ObjectFactory<ListDistribution> obj("list.distribution");
    obj.processData<DataTypeMList>();
}
