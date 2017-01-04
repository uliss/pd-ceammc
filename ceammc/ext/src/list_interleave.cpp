//
//  list_interleave.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 23/12/16.
//

#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

#include <algorithm>
#include <cassert>
#include <limits>

using namespace ceammc;

static const size_t MIN_INLET = 2;
static const size_t MAX_INLET = 20;

class ListInterleave : public BaseObject {
    size_t in_count_;
    std::vector<AtomList> in_list_;
    AtomList out_list_;

public:
    ListInterleave(const PdArgs& a)
        : BaseObject(a)
        , in_count_(math::clip(atomlistToValue<size_t>(a.args, MIN_INLET), MIN_INLET, MAX_INLET))
    {
        initLists();
        initInlets();
        createOutlet();
    }

    void onBang()
    {
        listTo(0, out_list_);
    }

    void onList(const AtomList& l)
    {
        in_list_[0] = l;
        processLists();
        onBang();
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n >= in_count_) {
            OBJ_ERR << "invalid inlet index: " << n << ", total inlet count = " << in_count_;
            return;
        }

        in_list_[n] = l;
    }

private:
    void initInlets()
    {
        assert(in_count_ > 1);

        for (size_t i = 0; i < in_count_ - 1; i++)
            createInlet();
    }

    void initLists()
    {
        in_list_.reserve(in_count_);

        for (size_t i = 0; i < in_count_; i++)
            in_list_.push_back(AtomList());
    }

    void processLists()
    {
        out_list_.clear();
        size_t min_size = shortestSize();
        if (min_size == 0)
            return;

        for (size_t i = 0; i < min_size; i++) {
            for (size_t j = 0; j < in_count_; j++) {
                out_list_.append(in_list_[j][i]);
            }
        }
    }

    size_t shortestSize() const
    {
        assert(in_count_ == in_list_.size());

        size_t res = std::numeric_limits<size_t>::max();

        for (size_t i = 0; i < in_count_; i++)
            res = std::min(res, in_list_[i].size());

        return res;
    }
};

extern "C" void setup_list0x2einterleave()
{
    ObjectFactory<ListInterleave> obj("list.interleave");
}
