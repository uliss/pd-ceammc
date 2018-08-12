#include "list_count.h"
#include "../data/data_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"

ListCount::ListCount(const PdArgs& a)
    : BaseObject(a)
    , pattern_(0)
{
    createInlet();
    createOutlet();

    pattern_ = new AtomProperty("@pattern", positionalArgument(0));
    createProperty(pattern_);
}

void ListCount::onList(const AtomList& l)
{
    floatTo(0, l.count(pattern_->value()));
}

void ListCount::onInlet(size_t n, const AtomList& l)
{
    if (l.size() < 1)
        return;
    else if (l.size() == 1)
        pattern_->setValue(l[0]);
    else {
        OBJ_DBG << "search pattern can be only single element, not list. Using first element from " << l;
        pattern_->setValue(l[0]);
    }
}

void ListCount::onDataT(const DataTypeMList& lst)
{
    onList(lst.toList());
}

void setup_list_count()
{
    ObjectFactory<ListCount> obj("list.count");
    obj.processData<DataTypeMList>();
}
