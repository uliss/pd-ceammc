/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "flt_median.h"
#include "ceammc_factory.h"

FltMedian::FltMedian(const PdArgs& args)
    : BaseObject(args)
    , window_size_(nullptr)
    , window_idx_(-1)
{
    window_size_ = new IntProperty("@size", 9);
    window_size_->checkClosedRange(1, 128);
    window_size_->setUnitsSamp();
    window_size_->setSuccessFn([this](Property* p) { updateSize(); });
    addProperty(window_size_);

    createOutlet();
}

void FltMedian::onFloat(t_float v)
{
    const bool FULL_WINDOW = window_idx_ < window_.size();

    // if full window
    if (FULL_WINDOW) {
        // remove oldest value from the pool.
        auto last = window_[window_idx_];
        auto last_pos = std::lower_bound(pool_.begin(), pool_.end(), last);
        if (last_pos != pool_.end()) {
            pool_.erase(last_pos);
        }
    }

    // insert into pool
    auto insert_pos = std::lower_bound(pool_.begin(), pool_.end(), v);
    pool_.insert(insert_pos, v);

    // write to buffer
    if (FULL_WINDOW)
        window_[window_idx_] = v;
    else
        window_.push_back(v);

    // increment circular index
    window_idx_ = (window_idx_ + 1) % window_size_->value();

    // median is always the middle of the pool
    if (pool_.size() % 2 == 1) { // odd
        auto idx = pool_.size() / 2;
        floatTo(0, pool_[idx]);
    } else { // even
        auto idx = pool_.size() / 2 - 1;
        floatTo(0, (pool_[idx] + pool_[idx + 1]) / 2.f);
    }
}

void FltMedian::updateSize()
{
    window_.clear();
    pool_.clear();
    window_idx_ = 0;
}

void setup_flt_median()
{
    ObjectFactory<FltMedian> obj("flt.median");

    obj.setDescription("Median control flow filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "median"});
}
