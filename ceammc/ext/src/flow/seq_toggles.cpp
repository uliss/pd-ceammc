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
#include "seq_toggles.h"
#include "ceammc_factory.h"

#include <algorithm>

static t_symbol* SYM_DUR;

SeqToggles::SeqToggles(const PdArgs& args)
    : SeqBangs(args)
    , length_(nullptr)
    , clock_off_([this]() { outputOff(); })
{
    length_ = new FloatProperty("@length", 0.75);
    length_->checkClosedRange(0, 1);
    addProperty(length_);
}

void SeqToggles::outputTick()
{
    const auto ms = calcNextTick();
    floatTo(1, current_);
    anyTo(1, SYM_DUR, Atom(ms));
    floatTo(0, 1);

    const t_float toff_ms = std::max<t_float>(1, ms * length_->value());
    clock_off_.delay(toff_ms);
}

void SeqToggles::stop()
{
    SeqBangs::stop();

    if (clock_off_.isActive()) {
        clock_off_.unset();
        outputOff();
    }
}

void SeqToggles::reset()
{
    SeqBangs::reset();

    if (clock_off_.isActive()) {
        clock_off_.unset();
        outputOff();
    }
}

void SeqToggles::outputOff()
{
    floatTo(0, 0);
}

void setup_seq_toggles()
{
    SYM_DUR = gensym("dur");

    ObjectFactory<SeqToggles> obj("seq.toggles");
    obj.addAlias("seq.t");

    obj.addMethod("stop", &SeqToggles::m_stop);
    obj.addMethod("reset", &SeqToggles::m_reset);
}
