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
#ifndef FLT_CALC_BIQUAD_H
#define FLT_CALC_BIQUAD_H

#include "ceammc_object.h"

#include <array>
using namespace ceammc;

struct FreqCalcParams {
    double freq_default, freq_min, freq_max;
    double ang_freq_default, ang_freq_min, ang_freq_max;
};

class FltCalcBiquad : public BaseObject {
protected:
    FloatProperty* freq_;
    FloatProperty* q_;
    FlagProperty* rad_;
    std::array<double, 3> b_;
    std::array<double, 3> a_;

public:
    FltCalcBiquad(const PdArgs& args, const FreqCalcParams& fparam);

    void onBang() override;
    void onFloat(t_float v) override;

    virtual void calc() = 0;

    double angleFreq() const;

    void m_bandwidth(t_symbol* s, const AtomListView& lv);

    void calc_hpf();
    void calc_lpf();
    void calc_onepole(bool hpf);
    void calc_notch();
    void calc_allpass();

private:
    void output();
    void normalizeA();
};

#endif // FLT_CALC_BIQUAD_H
