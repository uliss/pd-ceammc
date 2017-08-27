#include "list_choice.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"

#include <cstdlib>

extern "C" void setup_list0x2echoice()
{
    ObjectFactory<ListChoice> obj("list.choice");
    obj.mapFloatToList();
}

ListChoice::ListChoice(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void ListChoice::onList(const AtomList& l)
{
    if (l.empty()) {
        OBJ_ERR << "empty list given";
        return;
    }

    size_t idx = static_cast<size_t>(rand()) % l.size();
    atomTo(0, l[idx]);
}
