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
    addProperty(pattern_);
}

void ListEqual::onInlet(size_t n, const AtomListView& lst)
{
    if (n != 1)
        return;

    pattern_->set(lst);
}

void ListEqual::onList(const AtomList& lst)
{
    floatTo(0, lst == pattern_->value() ? 1 : 0);
}

void setup_list_equal()
{
    ObjectFactory<ListEqual> obj("list.equal");
    obj.processData<DataTypeMList>();
}
