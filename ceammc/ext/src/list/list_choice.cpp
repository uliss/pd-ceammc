#include "list_choice.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

#include <cstdlib>

const static int MAX_REPEAT_TRIES = 10;

ListChoice::ListChoice(const PdArgs& a)
    : ListBase(a)
    , no_repeat_(0)
    , prev_idx_(size_t(-1))
{
    // do not repeat index value (not the values itself in list!)
    no_repeat_ = new BoolProperty("@norepeat", false);
    addProperty(no_repeat_);

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

void setup_list_choice()
{
    ObjectFactory<ListChoice> obj("list.choice");
    obj.useDefaultPdFloatFn();
    obj.processData<DataTypeMList>();

    obj.setDescription("outputs random element from list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "random", "choice" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListChoice::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListChoice::setOutletsInfo(obj.classPointer(), { "atom" });
}
