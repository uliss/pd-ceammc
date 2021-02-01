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
#include "flt_calc_biquad.h"
#include "ceammc_filter.h"

FltCalcBiquad::FltCalcBiquad(const PdArgs& args, const FreqCalcParams& fparam)
    : BaseObject(args)
    , freq_(nullptr)
    , q_(nullptr)
    , rad_(nullptr)
{
    static t_symbol* PROP_RAD = gensym("@rad");
    const bool rad = args.args.contains(PROP_RAD);
    const auto freq_def = (rad ? fparam.ang_freq_default : fparam.freq_default);

    freq_ = new FloatProperty("@freq", freq_def);
    freq_->setArgIndex(0);
    addProperty(freq_);

    if (rad) {
        freq_->checkClosedRange(fparam.ang_freq_min, fparam.ang_freq_max);
        freq_->setUnits(PropValueUnits::RAD);
    } else {
        freq_->checkClosedRange(fparam.freq_min, fparam.freq_max);
        freq_->setUnits(PropValueUnits::HZ);
    }

    q_ = new FloatProperty("@q", M_SQRT1_2);
    q_->checkClosedRange(0.01, 100);
    addProperty(q_);

    rad_ = new FlagProperty(PROP_RAD->s_name);
    rad_->setInitOnly();
    addProperty(rad_);

    createOutlet();
}

void FltCalcBiquad::onBang()
{
    calc();
    output();
}

void FltCalcBiquad::onFloat(t_float v)
{
    if (!freq_->setValue(v))
        return;

    calc();
    output();
}

double FltCalcBiquad::angleFreq() const
{
    if (rad_->value())
        return freq_->value();
    else
        return flt::freq2ang(freq_->value(), sys_getsr());
}

void FltCalcBiquad::m_bandwidth(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    auto q = flt::bandwidth2q(lv[0].asT<t_float>(), angleFreq());
    if (!q_->setValue(q))
        return;
}

void FltCalcBiquad::calc_lpf()
{
    const double w = angleFreq();
    const auto cosw = std::cos(w);
    const auto sinw = std::sin(w);
    const auto alpha = sinw / (2 * q_->value());

    b_[0] = (1 - cosw) / 2;
    b_[1] = 1 - cosw;
    b_[2] = b_[0];
    a_[0] = 1 + alpha;
    a_[1] = -2 * cosw;
    a_[2] = 1 - alpha;

    normalizeA();
}

void FltCalcBiquad::calc_onepole(bool hpf)
{
    const double w = angleFreq();

    if (!hpf) { // lp
        const auto expw = std::exp(-w);
        b_[0] = 1 - expw;
        b_[1] = 0;
        b_[2] = 0;
        a_[0] = 1;
        a_[1] = -expw;
        a_[2] = 0;
    } else {
        const auto expw = std::exp(-(flt::m_pi - w));
        b_[0] = 1 - expw;
        b_[1] = 0;
        b_[2] = 0;
        a_[0] = 1;
        a_[1] = expw;
        a_[2] = 0;
    }
}

void FltCalcBiquad::calc_notch()
{
    const double w = angleFreq();
    setBA(flt::calc_notch<double>(w, q_->value()));
}

void FltCalcBiquad::calc_allpass()
{
    const double w = angleFreq();
    const auto cosw = std::cos(w);
    const auto sinw = std::sin(w);
    const auto a = sinw / (2 * q_->value());

    b_[0] = 1 - a;
    b_[1] = -2 * cosw;
    b_[2] = 1 + a;
    a_[0] = 1 + a;
    a_[1] = -2 * cosw;
    a_[2] = 1 - a;

    normalizeA();
}

void FltCalcBiquad::calc_hpf()
{
    const double w = angleFreq();
    const auto cosw = std::cos(w);
    const auto sinw = std::sin(w);
    const auto alpha = sinw / (2 * q_->value());

    b_[0] = (1 + cosw) / 2;
    b_[1] = -(1 + cosw);
    b_[2] = b_[0];
    a_[0] = 1 + alpha;
    a_[1] = -2 * cosw;
    a_[2] = 1 - alpha;

    normalizeA();
}

void FltCalcBiquad::output()
{
    Atom res[5];
    res[0] = b_[0];
    res[1] = b_[1];
    res[2] = b_[2];
    res[3] = a_[1];
    res[4] = a_[2];

    listTo(0, AtomListView(res, 5));
}

void FltCalcBiquad::normalizeA()
{
    const auto norm = a_[0];

    for (auto& v : b_)
        v /= norm;

    for (auto& v : a_)
        v /= norm;
}
