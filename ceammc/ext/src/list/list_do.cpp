#include "list_do.h"

ListDo::ListDo(const PdArgs& a)
    : BaseObject(a)
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

    for (size_t i = 0; i < l.size(); i++)
        mapped_list_.append(l[i]);
}

extern "C" void setup_list0x2edo()
{
    ObjectFactory<ListDo>("list.do");
}
