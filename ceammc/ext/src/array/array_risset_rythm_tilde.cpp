/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "array_risset_rythm_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

ArrayRissetRythmTilde::ArrayRissetRythmTilde(const PdArgs& args)
    : ArraySoundBase(args)
    , T_(0)
    , te_(0)
{
    bandwidth_ = new IntProperty("@b", 3);
    bandwidth_->checkClosedRange(1, 5);
    addProperty(bandwidth_);

    length_ = new IntProperty("@l", 2);
    length_->checkClosedRange(1, 8);
    addProperty(length_);

    createSignalOutlet();
}

void ArrayRissetRythmTilde::setupDSP(t_signal** sp)
{
    ArraySoundBase::setupDSP(sp);

    if (checkArray()) {
        T_ = array_.size();
        te_ = 0;
        tl0_ = 0;
        tl1_ = 0;
        tl2_ = 0;
        tl3_ = 0;
        tl4_ = 0;
    } else {
        T_ = 0;
        te_ = 0;
    }
}

void ArrayRissetRythmTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto bs = blockSize();

    if (T_ == 0) {
        for (size_t i = 0; i < bs; i++)
            out[0][i] = 0;

        return;
    }

    auto calc_rv_te = [](t_sample v, t_sample te, t_sample T, t_sample tau) { return ((T * std::log(2)) / tau) * std::exp2(te / tau + v); };
    auto calc_pr = [](t_sample b, t_sample r) { return 0.5 * (clip(std::cos((M_PI * b / 2) * std::log2(r)), -M_PI, M_PI) + 1); };

    const auto tau = T_ * length_->value();
    if (te_ >= tau)
        te_ = 0;

    const t_sample b = bandwidth_->value();

    for (size_t i = 0; i < bs; i++) {
        auto& x = out[0][i];
        x = 0;

        const auto r0 = calc_rv_te(0, te_ + i, T_, tau);
        x += array_.readSafe1(tl0_) * calc_pr(b, r0);
        tl0_ += r0;
        if (tl0_ >= T_) {
            tl0_ = 0;
        }

        const auto r1 = calc_rv_te(1, te_ + i, T_, tau);
        x += array_.readSafe1(tl1_) * calc_pr(b, r1);
        tl1_ += r1;
        if (tl1_ >= T_) {
            tl1_ = 0;
        }

        const auto r2 = calc_rv_te(2, te_ + i, T_, tau);
        x += array_.readSafe1(tl2_) * calc_pr(b, r2);
        tl2_ += r2;
        if (tl2_ >= T_) {
            tl2_ = 0;
        }

        const auto r3 = calc_rv_te(3, te_ + i, T_, tau);
        x += array_.readSafe1(tl3_) * calc_pr(b, r3);
        tl3_ += r3;
        if (tl3_ >= T_) {
            tl3_ = 0;
        }

        const auto r4 = calc_rv_te(4, te_ + i, T_, tau);
        x += array_.readSafe1(tl4_) * calc_pr(b, r4);
        tl4_ += r4;
        if (tl4_ >= T_) {
            tl4_ = 0;
        }
    }

    te_ += bs;
}

void setup_array_risset_rythm_tilde()
{
    SoundExternalFactory<ArrayRissetRythmTilde> obj("array.risset_rythm~");
}
