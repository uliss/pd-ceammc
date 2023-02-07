/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "array_tramp_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

template <class T>
std::pair<T, T> minmax_copy(const T a, const T b)
{
    return (b < a) ? std::make_pair(b, a) : std::make_pair(a, b);
}

ArrayTRampTilde::ArrayTRampTilde(const PdArgs& args)
    : ArraySoundBase(args)
    , speed_(nullptr)
    , from_(nullptr)
    , to_(nullptr)
    , phase_(0)
    , clock_([this]() {
        bangTo(1);
    })
    , is_running_(false)
{
    speed_ = new FloatProperty("@speed", 1);
    speed_->setArgIndex(1);
    addProperty(speed_);

    from_ = new FloatProperty("@from", 0);
    from_->checkClosedRange(0, 1);
    addProperty(from_);

    to_ = new FloatProperty("@to", 1);
    to_->checkClosedRange(0, 1);
    addProperty(to_);

    createSignalOutlet();
    createOutlet();
}

void ArrayTRampTilde::onBang()
{
    if (!checkArray())
        return;

    phase_ = rampStart();
    is_running_ = true;
}

void ArrayTRampTilde::setupDSP(t_signal** sp)
{
    ArraySoundBase::setupDSP(sp);
    phase_ = clip<double>(phase_, 0, array_.size() - 1);
}

void ArrayTRampTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();
    if (!is_running_)
        return std::fill(out[0], out[0] + BS, -std::numeric_limits<t_sample>::min());

    const auto dx = speed_->value();
    const auto range = minmax_copy(fromSamples(), toSamples());

    bool done = false;
    for (size_t i = 0; i < BS; i++) {
        if (phase_ > range.second) {
            out[0][i] = range.second;
            done = true;
        } else if (phase_ < range.first) {
            out[0][i] = range.first;
            done = true;
        } else {
            out[0][i] = phase_;
            phase_ += dx;
        }
    }

    if (done) {
        phase_ = rampStart();
        is_running_ = false;
        clock_.delay(0);
    }
}

size_t ArrayTRampTilde::rampStart() const
{
    const double ASIZE = array_.size();

    const auto mm = minmax_copy(from_->value(), to_->value());
    if (speed_->value() > 0)
        return std::round(mm.first * (ASIZE - 1));
    else
        return std::round(mm.second * (ASIZE - 1));
}

void setup_array_tramp_tilde()
{
    SoundExternalFactory<ArrayTRampTilde> obj("array.tramp~", OBJECT_FACTORY_DEFAULT);
}
