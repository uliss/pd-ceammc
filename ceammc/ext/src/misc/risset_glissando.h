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
#ifndef RISSET_GLISSANDO_H
#define RISSET_GLISSANDO_H

#include <cstdint>
#include <utility>

#include "ceammc_object.h"
using namespace ceammc;

class RissetCalc {
    t_float interval_ { 12 };
    t_float shift_ { 0 };
    t_float amp_exp_ = { 2 };
    uint8_t num_ { 5 };

public:
    struct IntAmp {
        t_float interval;
        t_float amp;
    };

public:
    RissetCalc(uint8_t num = 5, t_float interval = 12, t_float shift = 0, t_float amp_exp = 2);

    uint8_t num() const { return num_; }
    bool setNum(uint8_t n) noexcept;

    t_float interval() const { return interval_; }
    bool setInterval(t_float interval) noexcept;

    t_float shift() const { return shift_; }
    bool setShift(t_float shift) noexcept;

    t_float ampExponent() const { return amp_exp_; }
    bool setAmpExponent(t_float amp_exp) noexcept;

    bool calcSingle(t_float phase, uint8_t idx, IntAmp& data);
    bool calcMany(t_float phase, IntAmp* data, uint8_t n);
};

class RissetGlissando : public BaseObject {
    RissetCalc calc_;
    std::vector<RissetCalc::IntAmp> data_;
    FloatProperty* base_;

public:
    RissetGlissando(const PdArgs& args);
    void onFloat(t_float phase) override;
};

void setup_misc_risset_glissando();

#endif // RISSET_GLISSANDO_H
