#include "list_unpack.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

const static size_t MIN_OUTLETS = 1;
const static size_t MAX_OUTLETS = 32;
const static size_t DEFAULT_OUTLETS = 1;

ListUnpack::ListUnpack(const PdArgs& a)
    : BaseObject(a)
    , n_(0)
{
    n_ = new SizeTProperty("@n", DEFAULT_OUTLETS, true);
    createProperty(n_);
    n_->set(positionalArguments());
    n_->setValue(clip<size_t>(n_->value(), MIN_OUTLETS, MAX_OUTLETS));

    for (size_t i = 0; i < n_->value(); i++) {
        createOutlet();
    }
}

void ListUnpack::onList(const AtomList& l)
{
    for (size_t i = std::min(l.size(), n_->value()); i > 0; i--) {
        atomTo(i - 1, l[i - 1]);
    }
}

extern "C" void setup_list0x2eunpack()
{
    ObjectFactory<ListUnpack> obj("list.unpack");
}
