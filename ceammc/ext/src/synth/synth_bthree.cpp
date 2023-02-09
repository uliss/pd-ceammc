/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "synth_bthree.h"
#include "stk_synth_factory.h"

#include "BeeThree.h"
#include "stksynth_p.h"

using Synth = StkFMSynth<stk::BeeThree>;

SynthBThree::SynthBThree(const PdArgs& args)
    : StkSynth(args, new Synth())
{
    addProperty(new Synth::CCProperty("@op4", 2, *this));
    addProperty(new Synth::CCProperty("@op3", 4, *this));
    addProperty(new Synth::CCProperty("@lfo_speed", 11, *this));
    addProperty(new Synth::CCProperty("@lfo_depth", 1, *this));
    addProperty(new Synth::CCProperty("@adsr", 128, *this));
}

void setup_synth_bthree()
{
    StkSynthFactory<SynthBThree> obj("synth.bee3~");
    obj.addMethod("note", &SynthBThree::m_note);

    obj.setDescription("Hammond-oid organ FM synthesis instrument");
    obj.setCategory("synth");
    obj.setKeywords({"hammond", "synth", "fm"});
}
