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

static t_symbol* PROP_SIZE = gensym("@size");

static void updatePropSize(BaseObject* b, t_symbol* prop)
{
    if (prop != PROP_SIZE)
        return;

    auto obj = static_cast<FltMedian*>(b);
    obj->updateSize();
}

FltMedian::FltMedian(const PdArgs& args)
    : BaseObject(args)
    , window_size_(nullptr)
    , window_idx_(0)
{
    window_size_ = new IntPropertyClosedRange("@size", 9, 1, 128);
    createProperty(window_size_);

    setPropertyCallback(updatePropSize);

    createOutlet();
}

void FltMedian::onFloat(t_float v)
{
    // encrease if not full window
    if (window_.size() < window_size_->value())
        window_.push_back(v);

    // remove oldest value from the pool.
    auto last = window_[window_idx_];
    auto last_pos = std::lower_bound(pool_.begin(), pool_.end(), last);
    if (last_pos != pool_.end())
        pool_.erase(last_pos);

    // insert into pool
    auto insert_pos = std::lower_bound(pool_.begin(), pool_.end(), v);
    pool_.insert(insert_pos, v);

    // write to buffer
    window_[window_idx_] = v;
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
}
