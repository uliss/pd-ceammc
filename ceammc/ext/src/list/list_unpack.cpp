#include "list_unpack.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

const static size_t MIN_OUTLETS = 1;
const static size_t MAX_OUTLETS = 32;
const static size_t DEFAULT_OUTLETS = 1;

ListUnpack::ListUnpack(const PdArgs& a)
    : BaseObject(a)
    , n_(DEFAULT_OUTLETS)
{
    if (positionalArguments().size() > 0) {
        n_ = clip<size_t>(positionalArguments().asSizeT(DEFAULT_OUTLETS), MIN_OUTLETS, MAX_OUTLETS);
    }

    if (args().hasProperty("@n")) {
        Atom n;
        if (args().property("@n", &n)) {
            if (n.isFloat()) {
                n_ = clip<size_t>(n.asSizeT(DEFAULT_OUTLETS), MIN_OUTLETS, MAX_OUTLETS);
            }
        }
    }

    createCbProperty("@n", &ListUnpack::p_n);

    for (size_t i = 0; i < n_; i++) {
        createOutlet();
    }
}

void ListUnpack::onList(const AtomList& l)
{
    for (size_t i = std::min(l.size(), n_); i > 0; i--) {
        atomTo(i - 1, l[i - 1]);
    }
}

AtomList ListUnpack::p_n() const
{
    return AtomList(n_);
}

void ListUnpack::parseProperties()
{
}

extern "C" void setup_list0x2eunpack()
{
    ObjectFactory<ListUnpack> obj("list.unpack");
}
