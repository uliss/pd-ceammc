#include "list_unique.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListUnique::ListUnique(const ceammc::PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListUnique::onList(const AtomList& l)
{
    listTo(0, list::unique(l));
}

extern "C" void setup_list0x2eunique()
{
    ObjectFactory<ListUnique> obj("list.unique");
}
