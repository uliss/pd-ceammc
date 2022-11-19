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
#include "risset_glissando.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

RissetCalc::RissetCalc(uint8_t num, t_float interval, t_float shift, t_float amp_exp)
{
    setNum(num);
    setInterval(interval);
    setShift(shift);
    setAmpExponent(amp_exp);
}

bool RissetCalc::setNum(uint8_t n) noexcept
{
    if (n < 1)
        return false;

    num_ = n;
    return true;
}

bool RissetCalc::setInterval(t_float interval) noexcept
{
    if (interval < 1)
        return false;

    interval_ = interval;
    return true;
}

bool RissetCalc::setShift(t_float shift) noexcept
{
    shift_ = shift;
    return true;
}

bool RissetCalc::setAmpExponent(t_float amp_exp) noexcept
{
    if (amp_exp < 1 || amp_exp > 8)
        return false;

    amp_exp_ = amp_exp;
    return true;
}

bool RissetCalc::calcSingle(t_float phase, uint8_t idx, IntAmp& data)
{
    if (idx >= num_)
        return false;

    const auto normShift = shift_ / num_;
    const auto normIdx = t_float(idx) / num_;

    const auto x = wrapFloatMinMax<t_float>(phase + normIdx - normShift, -0.5, 0.5);
    data.amp = std::pow(std::cos(x * M_PI), amp_exp_);
    data.interval = interval_ * ((x + normShift) * num_);
    return true;
}

bool RissetCalc::calcMany(t_float phase, RissetCalc::IntAmp* data, uint8_t n)
{
    if (n > num_)
        return false;

    for (uint8_t i = 0; i < std::min(n, num_); i++) {
        if (!calcSingle(phase, i, data[i]))
            return false;
    }

    return true;
}

RissetGlissando::RissetGlissando(const PdArgs& args)
    : BaseObject(args)
    , base_(nullptr)
{
    base_ = new FloatProperty("@base", 60);
    base_->checkClosedRange(10, 100);
    base_->setArgIndex(0);
    base_->setUnits(PropValueUnits::SEMITONE);
    addProperty(base_);

    data_.resize(calc_.num());

    createCbIntProperty(
        "@n", [this]() -> int { return calc_.num(); },
        [this](int n) -> bool {
            bool res = calc_.setNum(n);
            if (!res)
                return res;

            data_.resize(n);
            return res;
        })
        ->setInitOnly();

    createCbFloatProperty(
        "@interval", [this]() -> t_float { return calc_.interval(); },
        [this](t_float interval) -> bool { return calc_.setInterval(interval); })
        ->setUnits(PropValueUnits::SEMITONE);

    createCbFloatProperty(
        "@shift", [this]() -> t_float { return calc_.shift(); },
        [this](t_float shift) -> bool { return calc_.setShift(shift); });

    createCbFloatProperty(
        "@ampexp", [this]() -> t_float { return calc_.ampExponent(); },
        [this](t_float ampExp) -> bool { return calc_.setAmpExponent(ampExp); });

    createOutlet();
    createOutlet();
}

void RissetGlissando::onFloat(t_float phase)
{
    if (!calc_.calcMany(phase, data_.data(), data_.size())) {
        OBJ_ERR << "calc error";
        return;
    }

    using AList = SmallAtomListN<8>;
    AList amp, notes;

    for (auto& d : data_) {
        amp.push_back(d.amp);
        notes.push_back(d.interval + base_->value());
    }

    listTo(1, amp.view());
    listTo(0, notes.view());
}

void setup_misc_risset_glissando()
{
    ObjectFactory<RissetGlissando> obj("risset.gliss");
}
