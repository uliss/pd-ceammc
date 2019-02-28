#include "list_equal.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"

ListEqual::ListEqual(const PdArgs& a)
    : BaseObject(a)
    , pattern_(nullptr)
{
    createInlet();
    createOutlet();

    pattern_ = new ListProperty("@pattern", a.args);
    createProperty(pattern_);
}

void ListEqual::onInlet(size_t n, const AtomList& lst)
{
    if (n != 1)
        return;

    pattern_->set(lst);
}

void ListEqual::onList(const AtomList& lst)
{
    floatTo(0, lst == pattern_->value() ? 1 : 0);
}

void ListEqual::onDataT(const DataTypeMList& lst)
{
    onList(lst.toList());
}

void setup_list_equal()
{
    ObjectFactory<ListEqual> obj("list.equal");
    obj.processData<DataTypeMList>();
}
