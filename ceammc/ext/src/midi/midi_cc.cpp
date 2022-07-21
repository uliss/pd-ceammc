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
#include "midi_cc.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

constexpr const char* STR_CTLIN = "#ctlin";

MidiCC::MidiCC(const PdArgs& args)
    : BaseObject(args)
    , cc_(nullptr)
    , chan_(nullptr)
    , min_(nullptr)
    , max_(nullptr)
    , curve_(nullptr)
{
    createOutlet();

    cc_ = new IntProperty("@cc", 0);
    cc_->checkClosedRange(0, 127);
    cc_->setArgIndex(0);
    addProperty(cc_);

    chan_ = new IntProperty("@ch", 0);
    chan_->checkClosedRange(0, 16);
    addProperty(chan_);

    min_ = new FloatProperty("@min", 0);
    min_->setArgIndex(1);
    addProperty(min_);

    max_ = new FloatProperty("@max", 127);
    max_->setArgIndex(2);
    addProperty(max_);

    curve_ = new FloatProperty("@curve", 0);
    addProperty(curve_);

    bindReceive(gensym(STR_CTLIN));
}

void MidiCC::onList(const AtomListView& lv)
{
    const auto cc = lv.intAt(0, 0);
    const auto val = lv.floatAt(1, 0);
    const auto chan = lv.intAt(2, 0);

    if (cc_->value() >= 0 && cc_->value() != cc)
        return;

    if (chan_->value() > 0 && chan_->value() != chan)
        return;

    const auto range = max_->value() - min_->value();
    if (curve_->value() == 0) {
        floatTo(0, val * range / 127 + min_->value());
    } else {
        auto f0 = convert::lin2curve(val, 0, 127, 0, 127, curve_->value());
        floatTo(0, f0 * range / 127 + min_->value());
    }
}

void setup_midi_cc()
{
    ObjectFactory<MidiCC> obj("midi.cc");
}
