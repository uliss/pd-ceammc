#include "list_do.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListDo::ListDo(const PdArgs& a)
    : ListBase(a)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListDo::onList(const AtomList& l)
{
    mapped_list_.clear();

    for (size_t i = 0; i < l.size(); i += 1) {
        AtomList ll(l[i]);
        ll.append(i);
        ll.append(l.size());
        listTo(1, ll);
    }

    listTo(0, mapped_list_);
}

void ListDo::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    if (l.empty())
        return;

    mapped_list_.append(l);
}

void setup_list_do()
{
    ObjectFactory<ListDo> obj("list.do");
    obj.processData<DataTypeMList>();
}
