/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef MIDI_NOTE_PASS_H
#define MIDI_NOTE_PASS_H

#include <memory>

#include "ceammc_object.h"
using namespace ceammc;

namespace mu {
class Parser;
}

class MidiNotePass : public BaseObject {
    std::unique_ptr<mu::Parser> expr_;
    double expr_note_ { 0 }, expr_vel_ { 0 };
    SymbolProperty* prop_expr_ { 0 };

public:
    MidiNotePass(const PdArgs& args);

    void initDone() final;
    void onList(const AtomListView& lv) final;
};

void setup_midi_note_pass();

#endif // MIDI_NOTE_PASS_H
