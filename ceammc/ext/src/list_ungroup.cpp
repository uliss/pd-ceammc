#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_object.h"

#include <algorithm>

using namespace ceammc;

const size_t MIN_GROUP_SIZE = 1;
const size_t MAX_GROUP_SIZE = 1024;

template <typename T>
static size_t checkedGroupSize(T v)
{
    return math::clip(static_cast<size_t>(v), MIN_GROUP_SIZE, MAX_GROUP_SIZE);
}

class ListUngroup : public BaseObject {
    SizeTPropertyClosedRange* group_size_;

public:
    ListUngroup(const PdArgs& a)
        : BaseObject(a)
        , group_size_(0)
    {
        createOutlet();
        createOutlet();

        group_size_ = new SizeTPropertyClosedRange("@size", MIN_GROUP_SIZE, MIN_GROUP_SIZE, MAX_GROUP_SIZE);
        createProperty(group_size_);

        parseArguments();
        if (!args().empty())
            group_size_->set(args());
    }

    void onList(const AtomList& l)
    {
        const size_t step = math::clip(group_size_->value(), MIN_GROUP_SIZE, MAX_GROUP_SIZE);
        for (size_t i = 0; i < l.size(); i += step) {
            listTo(1, l.slice(i, i + step, 1));
        }

        bangTo(0);
    }
};

extern "C" void setup_list0x2eungroup()
{
    ObjectFactory<ListUngroup> obj("list.ungroup");
}
