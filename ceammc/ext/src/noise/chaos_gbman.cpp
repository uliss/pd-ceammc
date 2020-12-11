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
#include "chaos_gbman.h"
#include "ceammc_factory.h"

#include <cmath>

constexpr t_float DEF_X = 1.2;
constexpr t_float DEF_Y = 2.1;
constexpr t_float MIN_FREQ = 0.001;
constexpr long MIN_PERIOD = 1;

static inline void gbman_next(double& xn, double& yn)
{
    //    x(n+1) = 1 - y(n) + |x(n)|
    //    y(n+1) = x(n)
    double xn0 = xn;
    xn = 1 - yn + std::fabs(xn0);
    yn = xn0;
}

ChaosGbman::ChaosGbman(const PdArgs& args)
    : BaseObject(args)
    , xn_(parsedPosArgs().floatAt(0, DEF_X))
    , yn_(parsedPosArgs().floatAt(1, DEF_Y))
{
    createOutlet();
    createOutlet();
}

void ChaosGbman::onBang()
{
    gbman_next(xn_, yn_);
    floatTo(1, yn_);
    floatTo(0, xn_);
}

void ChaosGbman::dump() const
{
    OBJ_DBG << "x: " << xn_;
    OBJ_DBG << "y: " << yn_;
    BaseObject::dump();
}

ChaosGbmanTilde::ChaosGbmanTilde(const PdArgs& args)
    : SoundExternal(args)
    , xn_(parsedPosArgs().floatAt(0, DEF_X))
    , yn_(parsedPosArgs().floatAt(1, DEF_Y))
    , counter_(0)
{
    createSignalOutlet();
    createSignalOutlet();
}

void ChaosGbmanTilde::processBlock(const t_sample** in, t_sample** out)
{
    const t_float freq = in[0][0];
    const size_t SR = samplerate();
    long call_period = (freq < SR) ? (SR / std::max<t_float>(freq, MIN_FREQ)) : MIN_PERIOD;
    call_period = std::max<long>(call_period, MIN_PERIOD);

    const auto BS = blockSize();
    for (size_t i = 0; i < BS; i++) {
        if (counter_ >= call_period) {
            counter_ -= call_period;
            gbman_next(xn_, yn_);
        }

        counter_++;
        out[0][i] = xn_;
        out[1][i] = yn_;
    }
}

void ChaosGbmanTilde::dump() const
{
    OBJ_DBG << "x: " << xn_;
    OBJ_DBG << "y: " << yn_;
    SoundExternal::dump();
}

void setup_chaos_gbman()
{
    ObjectFactory<ChaosGbman> obj("chaos.gbman0");
    obj.setDescription("Gingerbreadman map chaotic generator");
    obj.setCategory("chaos");
    obj.setKeywords({ "chaos", "generator", "gingerbreadman" });
    obj.addAuthor("Serge Poltavski");
    obj.setSinceVersion(0, 9);

    SoundExternalFactory<ChaosGbmanTilde> obj_tilde("chaos.gbman0~",
        OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_MAIN_SIGNAL_INLET);
    obj_tilde.setDescription("Gingerbreadman map chaotic generator");
    obj_tilde.setCategory("chaos");
    obj_tilde.addAuthor("Serge Poltavski");
    obj.setKeywords({ "chaos", "generator", "gingerbreadman" });
    obj_tilde.setSinceVersion(0, 9);
}
