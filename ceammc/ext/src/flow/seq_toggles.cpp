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
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

static t_symbol* SYM_BEAT_DUR;
static t_symbol* SYM_BEAT_LEN;
static t_symbol* SYM_IDX;

constexpr int MIN_NOTE_LEN = 1;

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
    const auto beat_dur_ms = calcNextTick();
    // setting minimal note length = 1ms
    const t_float beat_len_ms = clip_min<t_float, MIN_NOTE_LEN>(beat_dur_ms * length_->value());

    anyTo(1, SYM_IDX, Atom(current_));
    anyTo(1, SYM_BEAT_DUR, Atom(beat_dur_ms));
    anyTo(1, SYM_BEAT_LEN, Atom(beat_len_ms));
    floatTo(0, 1);

    // schedule off event
    clock_off_.delay(beat_len_ms);
}

void SeqToggles::clockStop()
{
    SeqBangs::clockStop();

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
    SYM_BEAT_DUR = gensym("bd");
    SYM_BEAT_LEN = gensym("bl");
    SYM_IDX = gensym("i");

    ObjectFactory<SeqToggles> obj("seq.toggles");
    obj.addAlias("seq.t");

    obj.addMethod("stop", &SeqToggles::m_stop);
    obj.addMethod("reset", &SeqToggles::m_reset);
}
