#include "list_choice.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

#include <cstdlib>
#include <ctime>
#include <random>

constexpr int MAX_REPEAT_TRIES = 10;

static std::mt19937 gen(std::time(nullptr));

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

void ListChoice::onList(const AtomListView& lv)
{
    const size_t sz = lv.size();

    if (sz == 0) {
        OBJ_ERR << "empty list given";
        return;
    }

    std::uniform_int_distribution<size_t> dist(0, sz - 1);
    auto idx = dist(gen);

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
                idx = dist(gen);
                if (++n_tries > MAX_REPEAT_TRIES) {
                    OBJ_DBG << "max @norepeat tries exceeded";
                    break;
                }
            }
        }
    }

    prev_idx_ = idx;
    atomTo(0, lv[idx]);
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
