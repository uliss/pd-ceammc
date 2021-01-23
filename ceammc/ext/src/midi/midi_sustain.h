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
#ifndef MIDI_SUSTAIN_H
#define MIDI_SUSTAIN_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "ceammc_proxy.h"

#include <bitset>

using namespace ceammc;

class MidiSustain : public BaseObject {
    using Proxy = PdListProxy<MidiSustain>;

private:
    std::bitset<128> notes_;
    BoolProperty* on_;
    BoolProperty* ctlin_;
    Proxy proxy_;

public:
    MidiSustain(const PdArgs& args);

    void onList(const AtomList& lst) override;
    void onInlet(size_t n, const AtomListView& lv) override;

    void onCC(const AtomListView& lv);

private:
    void notesOff();
};

void setup_midi_sustain();

#endif // MIDI_SUSTAIN_H
