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
        , group_size_(nullptr)
    {
        createOutlet();
        createOutlet();

        group_size_ = new IntProperty("@size", DEF_GROUP_SIZE);
        group_size_->checkClosedRange(MIN_GROUP_SIZE, MAX_GROUP_SIZE);
        group_size_->setArgIndex(0);
        addProperty(group_size_);
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

void setup_list_repack()
{
    ObjectFactory<ListRepack> obj("list.repack");

    obj.setDescription("repack list to specified size");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "repack" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListRepack::setInletsInfo(obj.classPointer(), { "list" });
    ListRepack::setOutletsInfo(obj.classPointer(), { "flow: repacked lists", "bang: when finished" });
}
