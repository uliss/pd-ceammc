#include "list_count.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

#include <algorithm>

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

void ListCount::onList(const AtomListView& lv)
{
    auto c = std::count(lv.begin(), lv.end(), pattern_->value());
    floatTo(0, c);
}

void ListCount::onInlet(size_t n, const AtomListView& l)
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

    obj.setDescription("output the number of items that equal to specified value");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "count" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListCount::setInletsInfo(obj.classPointer(), { "list or Mlist", "atom: pattern to compare" });
    ListCount::setOutletsInfo(obj.classPointer(), { "int" });
}
