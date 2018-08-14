#include "list_unpack.h"
#include "../data/datatype_mlist.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

const static size_t MIN_OUTLETS = 1;
const static size_t MAX_OUTLETS = 32;
const static size_t DEFAULT_OUTLETS = 1;

ListUnpack::ListUnpack(const PdArgs& a)
    : BaseObject(a)
    , n_(clip<int>(positionalFloatArgument(0, DEFAULT_OUTLETS), MIN_OUTLETS, MAX_OUTLETS))
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

void ListUnpack::onDataT(const DataTypeMList& l)
{
    const size_t N = std::min<size_t>(l.size(), n_);
    for (size_t i = N; i > 0; i--)
        atomTo(i - 1, l[i - 1].toAtom());
}

void setup_list_unpack()
{
    ObjectFactory<ListUnpack> obj("list.unpack");
    obj.processData<DataTypeMList>();
}
