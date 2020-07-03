#include "list_count.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

ListCount::ListCount(const PdArgs& a)
    : ListBase(a)
    , pattern_(nullptr)
{
    createInlet();
    createOutlet();

    pattern_ = new AtomProperty("@pattern", Atom());
    pattern_->setArgIndex(0);
    addProperty(pattern_);
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

void setup_list_count()
{
    ObjectFactory<ListCount> obj("list.count");
    obj.processData<DataTypeMList>();
}
