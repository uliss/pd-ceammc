#include "list_equal.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"

ListEqual::ListEqual(const PdArgs& a)
    : ListBase(a)
    , pattern_(nullptr)
{
    createInlet();
    createOutlet();

    pattern_ = new ListProperty("@pattern", a.args);
    pattern_->setArgIndex(0);
    addProperty(pattern_);
}

void ListEqual::onInlet(size_t n, const AtomListView& lv)
{
    if (n != 1)
        return;

    pattern_->set(lv);
}

void ListEqual::onList(const AtomListView& lv)
{
    boolTo(0, lv == pattern_->value());
}

void setup_list_equal()
{
    ObjectFactory<ListEqual> obj("list.equal");
    obj.processData<DataTypeMList>();

    obj.setDescription("checks if given lists are equal");
    obj.setCategory("list");
    obj.setKeywords({"list", "compare", "equal"});
}
