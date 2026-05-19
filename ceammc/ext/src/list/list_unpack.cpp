#include "list_unpack.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"
#include "fmt/core.h"

#include <algorithm>
#include <cstdio>

std::array<std::string, ListUnpack::MAX_OUTLETS> ListUnpack::outlets_info_;

ListUnpack::ListUnpack(const PdArgs& a)
    : BaseObject(a)
    , n_(positionalConstant<DEFAULT_OUTLETS, MIN_OUTLETS, MAX_OUTLETS>(0))
{
    for (size_t i = 0; i < n_; i++)
        createOutlet();
}

void ListUnpack::onList(const AtomListView& lv)
{
    const size_t N = std::min<size_t>(lv.size(), n_);

    for (size_t i = N; i > 0; i--)
        atomTo(i - 1, lv[i - 1]);
}

void ListUnpack::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

const char* ListUnpack::annotateOutlet(size_t n) const
{
    return n < outlets_info_.size() ? outlets_info_[n].c_str() : "?";
}

void ListUnpack::initOutletsInfo()
{
    for (size_t i = 0; i < MAX_OUTLETS; i++)
        outlets_info_[i] = fmt::format("\\[{}\\]", i);
}

void setup_list_unpack()
{
    ObjectFactory<ListUnpack> obj("list.unpack");
    obj.processData<DataTypeMList>();

    obj.setDescription("unpack list elements to separate outlets");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "unpack" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 3);

    ListUnpack::initOutletsInfo();
    ListUnpack::setInletsInfo(obj.classPointer(), { "list or Mlist" });
}
