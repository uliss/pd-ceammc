/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "chaos_standard.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cfenv>
#include <cmath>

constexpr double PI = 3.141592653589793;
constexpr double TWO_PI = 2 * PI;
constexpr double RECIP_2PI = 1 / TWO_PI;

constexpr t_float DEF_K = 1;
constexpr t_float DEF_X = 0.5;
constexpr t_float DEF_Y = 0;

static inline t_sample wrap_pi(t_sample x)
{
    return x * RECIP_2PI;
}

static inline void standard_next(double k, double& xn, double& yn)
{
    /// y(n+1) = (y(n) + k * sin(x(n))) % 2pi
    auto yn1 = std::fmod(yn + k * std::sin(xn), TWO_PI);

    /// x(n+1) = (x(n) + y(n+1)) % 2pi
    auto xn1 = std::fmod(xn + yn1, TWO_PI);

    if (!std::fetestexcept(FE_INVALID)) {
        xn = xn1;
        yn = yn1;
    }

    std::feclearexcept(FE_ALL_EXCEPT);
}

ChaosStandard::ChaosStandard(const PdArgs& args)
    : BaseObject(args)
    , k_(parsedPosArgs().floatAt(0, DEF_K))
    , xn_(parsedPosArgs().floatAt(1, DEF_X))
    , yn_(parsedPosArgs().floatAt(2, DEF_Y))
{
    createInlet(&k_);
    createOutlet();
    createOutlet();
}

void ChaosStandard::onBang()
{
    standard_next(k_, xn_, yn_);
    floatTo(1, wrap_pi(yn_));
    floatTo(0, wrap_pi(xn_));
}

ChaosStandardTilde::ChaosStandardTilde(const PdArgs& args)
    : SoundExternal(args)
    , xn_(parsedPosArgs().floatAt(0, DEF_X))
    , yn_(parsedPosArgs().floatAt(1, DEF_Y))
    , counter_(0)
{
    createSignalInlet();
    createSignalOutlet();
    createSignalOutlet();
}

void ChaosStandardTilde::processBlock(const t_sample** in, t_sample** out)
{
    t_float freq = in[0][0];
    t_float call_period = (freq < samplerate()) ? (samplerate() / std::max<t_float>(freq, 0.001)) : 1;
    call_period = std::max<t_float>(call_period, 1);

    const auto BS = blockSize();
    for (size_t i = 0; i < BS; i++, counter_++) {
        if (counter_ >= call_period) {
            counter_ -= call_period;

            auto k = in[1][i];
            standard_next(k, xn_, yn_);
        }

        out[0][i] = wrap_pi(xn_);
        out[1][i] = wrap_pi(yn_);
    }
}

void setup_chaos_standard()
{
    ObjectFactory<ChaosStandard> obj("chaos.std0");
    SoundExternalFactory<ChaosStandardTilde> obj_tilde("chaos.std0~", OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_MAIN_SIGNAL_INLET);
}
