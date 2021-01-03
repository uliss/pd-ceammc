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
#ifndef MIDI_SYSEX_H
#define MIDI_SYSEX_H

#include "ceammc_object.h"
#include "ceammc_proxy.h"

#include <cstdint>

using namespace ceammc;

class MidiSysex : public BaseObject {
    using Proxy = PdListProxy<MidiSysex>;
    Proxy proxy_;
    std::vector<uint8_t> buffer_;

    enum State {
        IS_SYSEX,
        NOT_SYSEX
    };

    State state_;

public:
    MidiSysex(const PdArgs& args);

    void onSysex(const AtomListView& lv);
    void m_reset(t_symbol*, const AtomListView&);

private:
    void output();
};

void setup_midi_sysex();

#endif // MIDI_SYSEX_H
