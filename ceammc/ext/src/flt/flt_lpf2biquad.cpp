/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "flt_lpf2biquad.h"
#include "ceammc_factory.h"
#include "flt_common.h"

constexpr t_float Q_DEFAULT = M_SQRT1_2;
constexpr t_float FREQ_DEFAULT = 20;

FltLpf2Biquad::FltLpf2Biquad(const PdArgs& args)
    : BaseObject(args)
    , freq_(nullptr)
    , q_(nullptr)
    , bw_(nullptr)
    , norm_(nullptr)
{
    freq_ = new FloatProperty("@freq", FREQ_DEFAULT);
    freq_->setUnitsHz();

    const auto sr = (sys_getsr() <= 0) ? 48000 : sys_getsr();
    freq_->checkClosedRange(20, sr / 2);
    freq_->setArgIndex(0);
    addProperty(freq_);

    q_ = new FloatProperty("@q", Q_DEFAULT);
    q_->checkClosedRange(0.01, 100);
    q_->setSuccessFn([this](Property*) {
        auto w = flt::hz2w(freq_->value(), sys_getsr());
        auto bw = flt::q2bandwidth(q_->value(), w);
        bw_->setFloat(bw);
    });
    addProperty(q_);

    bw_ = new FloatProperty("@bw", flt::q2bandwidth(Q_DEFAULT, flt::hz2w(FREQ_DEFAULT, sr)));
    bw_->checkMin(0);
    bw_->setSuccessFn([this](Property*) {
        auto w = flt::hz2w(freq_->value(), sys_getsr());
        auto q = flt::bandwidth2q(bw_->value(), w);
        q_->setValue(q);
    });
    addProperty(bw_);

    norm_ = new BoolProperty("@norm", true);
    addProperty(norm_);

    createOutlet();
}

void FltLpf2Biquad::onBang()
{
    calc();
    output();
}

void FltLpf2Biquad::onFloat(t_float v)
{
    if (!freq_->setValue(v))
        return;

    calc();
    output();
}

void FltLpf2Biquad::calc()
{
    constexpr double pi = M_PI;
    const double Fs = sys_getsr();
    const double w = (2 * pi * freq_->value()) / Fs;
    const auto cosw = std::cos(w);
    const auto sinw = std::sin(w);
    const auto alpha = sinw / (2 * q_->value());

    b_[0] = (1 - cosw) / 2;
    b_[1] = 1 - cosw;
    b_[2] = b_[0];
    a_[0] = 1 + alpha;
    a_[1] = -2 * cosw;
    a_[2] = 1 - alpha;

    if (norm_->value()) {
        const auto norm = a_[0];

        for (auto& v : b_)
            v /= norm;

        for (auto& v : a_)
            v /= norm;
    }
}

void FltLpf2Biquad::output()
{
    Atom res[6];
    res[0] = b_[0];
    res[1] = b_[1];
    res[2] = b_[2];

    if (norm_->value()) {
        res[3] = a_[1];
        res[4] = a_[2];
        listTo(0, AtomListView(res, 5));
    } else {
        res[3] = a_[0];
        res[4] = a_[1];
        res[5] = a_[2];
        listTo(0, AtomListView(res, 6));
    }
}

void setup_flt_lpf2biquad()
{
    ObjectFactory<FltLpf2Biquad> obj("flt.c_lpf");
    obj.addAlias("lpf->biquad");
}
