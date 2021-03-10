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
#ifndef MIDI_CLOCK_H
#define MIDI_CLOCK_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "ceammc_proxy.h"
using namespace ceammc;

class MidiClock : public BaseObject {
    using Proxy = PdListProxy<MidiClock>;

private:
    Proxy proxy_;
    double tprev_;
    uint16_t beat_counter_;
    IntEnumProperty* div_;

public:
    MidiClock(const PdArgs& args);

    void onClock(const AtomListView& lv);
    void m_reset(t_symbol*, const AtomListView&);
};

void setup_midi_clock();

#endif // MIDI_CLOCK_H
