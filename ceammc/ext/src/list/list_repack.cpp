#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

constexpr int DEF_GROUP_SIZE = 1;
constexpr int MIN_GROUP_SIZE = 1;
constexpr int MAX_GROUP_SIZE = 1024;

class ListRepack : public BaseObject {
    IntProperty* group_size_;

public:
    ListRepack(const PdArgs& a)
        : BaseObject(a)
        , group_size_(0)
    {
        createOutlet();
        createOutlet();

        group_size_ = new IntProperty("@size", DEF_GROUP_SIZE);
        group_size_->checkClosedRange(MIN_GROUP_SIZE, MAX_GROUP_SIZE);
        createProperty(group_size_);

        if (checkArgs(positionalArguments(), ARG_FLOAT))
            group_size_->set(positionalArguments());
    }

    void onList(const AtomList& l)
    {
        const size_t step = clip<size_t>(group_size_->value(), MIN_GROUP_SIZE, MAX_GROUP_SIZE);
        for (size_t i = 0; i < l.size(); i += step) {
            listTo(0, l.slice(i, i + step - 1, 1));
        }

        bangTo(1);
    }
};

extern "C" void setup_list0x2erepack()
{
    ObjectFactory<ListRepack> obj("list.repack");
}
