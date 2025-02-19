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
#include "synth_sitar.h"
#include "stk_synth_factory.h"

#include "Sitar.h"
#include "stksynth_p.h"

using Synth = stk::Sitar;

SynthSitar::SynthSitar(const PdArgs& args)
    : StkSynth(args, new Synth())
{
}

void setup_synth_sitar()
{
    StkSynthFactory<SynthSitar> obj("synth.sitar~");
    obj.addMethod("note", &SynthSitar::m_note);

    obj.setDescription("STK sitar");
    obj.setCategory("synth");
    obj.setKeywords({"sitar", "string", "pluck", "karplus"});
}
