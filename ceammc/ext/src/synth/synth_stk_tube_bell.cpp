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
#include "synth_stk_tube_bell.h"
#include "stk_synth_factory.h"

#include "TubeBell.h"
#include "stksynth_p.h"

using Synth = StkFMSynth<stk::TubeBell>;

SynthTubeBell::SynthTubeBell(const PdArgs& args)
    : StkSynth(args, new Synth())
{
    addProperty(new Synth::CCProperty("@mod", 2, *this));
    addProperty(new Synth::CCProperty("@xfade", 4, *this));
    addProperty(new Synth::CCProperty("@lfo_speed", 11, *this));
    addProperty(new Synth::CCProperty("@lfo_depth", 1, *this));
    addProperty(new Synth::CCProperty("@adsr", 128, *this));

    property("@freq")->setArgIndex(0);
}

void setup_synth_tube_bell()
{
    StkSynthFactory<SynthTubeBell> obj("synth.tube_bell~");
    obj.addMethod("note", &SynthTubeBell::m_note);

    obj.setDescription("STK tubular bell (orchestral chime) FM synthesis instrument");
    obj.setCategory("synth");
    obj.setKeywords({"tubular", "stk", "bell"});
}
