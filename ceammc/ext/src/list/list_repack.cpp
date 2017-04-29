#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

static const size_t MIN_GROUP_SIZE = 1;
static const size_t MAX_GROUP_SIZE = 1024;

template <typename T>
static size_t checkedGroupSize(T v)
{
    return math::clip(static_cast<size_t>(v), MIN_GROUP_SIZE, MAX_GROUP_SIZE);
}

class ListRepack : public BaseObject {
    SizeTPropertyClosedRange* group_size_;

public:
    ListRepack(const PdArgs& a)
        : BaseObject(a)
        , group_size_(0)
    {
        createOutlet();
        createOutlet();

        group_size_ = new SizeTPropertyClosedRange("@size", MIN_GROUP_SIZE, MIN_GROUP_SIZE, MAX_GROUP_SIZE);
        createProperty(group_size_);

        if (checkArgs(positionalArguments(), ARG_FLOAT))
            group_size_->set(positionalArguments());
    }

    void onList(const AtomList& l)
    {
        const size_t step = math::clip(group_size_->value(), MIN_GROUP_SIZE, MAX_GROUP_SIZE);
        for (size_t i = 0; i < l.size(); i += step) {
            listTo(0, l.slice(i, i + step, 1));
        }

        bangTo(1);
    }
};

extern "C" void setup_list0x2erepack()
{
    ObjectFactory<ListRepack> obj("list.repack");
}
