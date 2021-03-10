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
#include "midi_clock.h"
#include "ceammc_factory.h"

static t_symbol* SYM_MIDIRT;

constexpr uint16_t PPQN = 24;

static inline uint16_t enumToDiv(int v)
{
    if (v == 0)
        return 1;
    else
        return 4 * PPQN / v;
}

MidiClock::MidiClock(const PdArgs& args)
    : BaseObject(args)
    , proxy_(this, &MidiClock::onClock)
    , tprev_(-1)
    , beat_counter_(0)
    , div_(nullptr)
{
    createOutlet();
    createOutlet();

    proxy_.bind(SYM_MIDIRT);

    div_ = new IntEnumProperty("@div", { 0, 4, 8, 16, 32 });
    div_->setArgIndex(0);
    addProperty(div_);
}

void MidiClock::onClock(const AtomListView& lv)
{
    if (lv.size() != 2)
        return;

    const auto portno = lv[0].asT<int>();
    const auto byte = lv[1].asT<int>();

    if (byte != 0xF8)
        return;

    if (beat_counter_ == 0) {
        if (tprev_ > 0) {
            auto ms = clock_gettimesince(tprev_);
            floatTo(1, 60000 / ms);
        }

        tprev_ = clock_getlogicaltime();
    }

    // output bang at specified division
    const auto div = enumToDiv(div_->value());
    if (beat_counter_ % div == 0)
        bangTo(0);

    beat_counter_++;
    beat_counter_ %= 24;
}

void MidiClock::m_reset(t_symbol*, const AtomListView&)
{
    beat_counter_ = 0;
}

void setup_midi_clock()
{
    SYM_MIDIRT = gensym("#midirealtimein");

    ObjectFactory<MidiClock> obj("midi.clock");

    obj.addMethod("reset", &MidiClock::m_reset);
    obj.setXletsInfo({ "control inlet" }, { "bang: on midi clock message", "float: measured tempo in bpm" });
}
