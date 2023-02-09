/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "vline_to_env.h"
#include "ceammc_factory.h"

Vline2Env::Vline2Env(const PdArgs& args)
    : BaseObject(args)
    , time_(clock_getlogicaltime())
    , len_(0)
    , v_(0)
    , clock_(this, &Vline2Env::tick)
{
    createOutlet();
}

void Vline2Env::onBang()
{
    atomTo(0, EnvAtom(env_));
}

void Vline2Env::onList(const AtomListView& lv)
{
    if (lv.empty())
        onBang();

    auto tm = clock_getlogicaltime();
    if (tm != time_) {
        env_.clear();
        time_ = tm;
        len_ = 0;
        v_ = 0;
    }

    // value, dur, offset
    auto v = lv.floatAt(0, 0);
    size_t dur_us = lv.floatAt(1, 0) * 1000;
    size_t off_us = lv.floatAt(2, 0) * 1000;

    // add prev value
    env_.insertPoint(EnvelopePoint(len_, v_, false, CURVE_LINE));
    // connecting step line
    if (off_us > len_) {
        env_.back().type = CURVE_STEP;
        env_.insertPoint(EnvelopePoint(off_us, v_, false, CURVE_LINE));
    }

    v_ = v;
    len_ = off_us + dur_us;
    clock_.delay(0);
}

void Vline2Env::tick()
{
    env_.insertPoint(EnvelopePoint(len_, v_, false, CURVE_LINE));
    onBang();
}

void setup_vline_to_env()
{
    ObjectFactory<Vline2Env> obj("vline2env");
    obj.addAlias("vline->env");

    obj.useDefaultPdFloatFn();

    obj.setDescription("Converts vline messages to env");
    obj.setCategory("env");
    obj.setKeywords({"envelope", "vline"});
}
