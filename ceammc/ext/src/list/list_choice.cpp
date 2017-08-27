#include "list_choice.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"

#include <cstdlib>

const static int MAX_REPEAT_TRIES = 10;

extern "C" void setup_list0x2echoice()
{
    ObjectFactory<ListChoice> obj("list.choice");
    obj.mapFloatToList();
}

ListChoice::ListChoice(const PdArgs& a)
    : BaseObject(a)
    , no_repeat_(0)
    , prev_idx_(size_t(-1))
{
    // do not repeat index value (not the values itself in list!)
    no_repeat_ = new BoolProperty("@norepeat", false);
    createProperty(no_repeat_);

    createOutlet();
}

void ListChoice::onList(const AtomList& l)
{
    const size_t sz = l.size();

    if (sz == 0) {
        OBJ_ERR << "empty list given";
        return;
    }

    size_t idx = static_cast<size_t>(rand()) % sz;

    // repeat index found
    if (no_repeat_->value() && prev_idx_ == idx) {
        if (sz < 2) {
            OBJ_ERR << "invalid list size for @norepeat property: " << sz;
            return;
        } else if (sz == 2) {
            idx = (idx + 1) % 2;
        } else {
            int n_tries = 0;
            while (prev_idx_ == idx) {
                idx = static_cast<size_t>(rand()) % sz;
                if (++n_tries > MAX_REPEAT_TRIES) {
                    OBJ_DBG << "max @norepeat tries exceeded";
                    break;
                }
            }
        }
    }

    prev_idx_ = idx;
    atomTo(0, l[idx]);
}
