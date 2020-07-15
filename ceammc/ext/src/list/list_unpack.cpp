#include "list_unpack.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

#include <algorithm>
#include <cstdio>

constexpr size_t MIN_OUTLETS = 1;
constexpr size_t MAX_OUTLETS = 32;
constexpr size_t DEFAULT_OUTLETS = 1;

ListUnpack::ListUnpack(const PdArgs& a)
    : BaseObject(a)
    , n_(positionalConstant<DEFAULT_OUTLETS, MIN_OUTLETS, MAX_OUTLETS>(0))
{
    for (size_t i = 0; i < n_; i++)
        createOutlet();
}

void ListUnpack::onList(const AtomList& l)
{
    const size_t N = std::min<size_t>(l.size(), n_);

    for (size_t i = N; i > 0; i--)
        atomTo(i - 1, l[i - 1]);
}

void ListUnpack::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

const char* ListUnpack::annotateOutlet(size_t n) const
{
    static std::vector<std::string> out_info_;

    if (n >= out_info_.size()) {
        out_info_.reserve(n + 1 - out_info_.size());
        char buf[32];
        for (size_t i = out_info_.size(); i <= n; i++) {
            sprintf(buf, "\\[%d\\]", (int)i);
            out_info_.push_back(buf);
        }
    }

    return out_info_[n].c_str();
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

    ListUnpack::setInletsInfo(obj.classPointer(), { "list or Mlist" });
}
