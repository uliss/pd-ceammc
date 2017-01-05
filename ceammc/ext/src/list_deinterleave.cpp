//
//  list_deinterleave.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 23/12/16.
//
//

#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

#include <cassert>

using namespace ceammc;

static const size_t MIN_OUTLET = 2;
static const size_t MAX_OUTLET = 20;

class ListDeinterleave : public BaseObject {
    const size_t out_count_;
    AtomList in_list_;
    std::vector<AtomList> out_lists_;

public:
    ListDeinterleave(const PdArgs& a)
        : BaseObject(a)
        , out_count_(math::clip(atomlistToValue<size_t>(a.args, MIN_OUTLET), MIN_OUTLET, MAX_OUTLET))
    {
        for (size_t i = 0; i < out_count_; i++) {
            out_lists_.push_back(AtomList());
            createOutlet();
        }
    }

    void onBang()
    {
        for (size_t i = out_lists_.size(); i > 0; i--)
            listTo(i - 1, out_lists_[i - 1]);
    }

    void onList(const AtomList& l)
    {
        clearOutputList();

        for (size_t i = 0; i < l.size(); i++) {
            size_t ndx = i % out_count_;
            out_lists_[ndx].append(l[i]);
        }

        onBang();
    }

private:
    void clearOutputList()
    {
        for (size_t i = 0; i < out_lists_.size(); i++)
            out_lists_[i].clear();
    }
};

extern "C" void setup_list0x2edeinterleave()
{
    ObjectFactory<ListDeinterleave> obj("list.deinterleave");
}
