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
#include "midi_velocity_ramp.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cmath>

MidiVelocityRamp::MidiVelocityRamp(const PdArgs& args)
    : BaseObject(args)
    , from_(nullptr)
    , to_(nullptr)
    , n_(nullptr)
    , auto_(nullptr)
    , counter_(0)
{
    createOutlet();
    createOutlet();

    from_ = new IntProperty("@from", 127);
    from_->checkClosedRange(0, 127);
    from_->setArgIndex(0);
    addProperty(from_);

    to_ = new IntProperty("@to", 127);
    to_->checkClosedRange(0, 127);
    to_->setArgIndex(1);
    addProperty(to_);

    n_ = new IntProperty("@n", 0);
    n_->checkMinEq(0);
    n_->setArgIndex(2);
    addProperty(n_);

    auto_ = new BoolProperty("@auto", false);
    addProperty(auto_);
}

void MidiVelocityRamp::onFloat(t_float v)
{
    if (v > 0)
        onList(StaticAtomList<2> { v, 127 }.view());
    else
        onList(StaticAtomList<2> { -v, 0. }.view());
}

void MidiVelocityRamp::onList(const AtomListView& lv)
{
    if (lv.size() < 1 || lv.size() > 2) {
        OBJ_ERR << "NOTE [VELOCITY] is expected, got: " << lv;
        return;
    }

    AtomList note(lv);
    note.resizeClip(2);

    if (note.intAt(1, 0) == 0)
        listTo(0, note);
    else if (n_->value() > 0) {
        const auto fval = convert::lin2lin_clip<t_float>(counter_, 0, n_->value(), from_->value(), to_->value());
        note[1] = std::round(fval);

        if (counter_ == n_->value())
            bangTo(1);

        counter_++;

        if (auto_->value()) // wrap to [0..n] range
            counter_ = counter_ % (n_->value() + 1);

        listTo(0, note);
    } else {
        note[1] = from_->value();
        listTo(0, note);
    }
}

void MidiVelocityRamp::m_reset(t_symbol* s, const AtomListView& lv)
{
    counter_ = 0;
}

void setup_midi_vel_ramp()
{
    ObjectFactory<MidiVelocityRamp> obj("midi.vramp");
    obj.addMethod("reset", &MidiVelocityRamp::m_reset);

    obj.setXletsInfo({ "float: midi note\n"
                       "list: note vel" },
        { "list: note vel", "bang: when last value is reached" });
}
