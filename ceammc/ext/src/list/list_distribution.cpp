#include "list_distribution.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListDistribution::ListDistribution(const PdArgs& a)
    : ListBase(a)
    , normalize_(nullptr)
{
    createOutlet();

    normalize_ = new BoolProperty("@normalize", true);
    addProperty(normalize_);
}

void ListDistribution::onList(const AtomListView& lv)
{
    listTo(0, list::countRepeats(lv, normalize_->value()));
}

void setup_list_distribution()
{
    ObjectFactory<ListDistribution> obj("list.distribution");
    obj.processData<DataTypeMList>();

    obj.setDescription("list value distribution");
    obj.setCategory("list");
    obj.setKeywords({"list", "distribution"});
}
