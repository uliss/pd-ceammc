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
#include "ceammc_factory.h"

#include "BeeThree.h"
#include "stksynth_p.h"

typedef StkFMSynth<stk::BeeThree> Synth;
typedef ControlChangeProperty<Synth> CCProperty;

SynthBThree::SynthBThree(const PdArgs& args)
    : StkSynth(args, new Synth())
{
    createProperty(new CCProperty("@op4", 2, *this));
    createProperty(new CCProperty("@op3", 4, *this));
    createProperty(new CCProperty("@lfo_speed", 11, *this));
    createProperty(new CCProperty("@lfo_depth", 1, *this));
    createProperty(new CCProperty("@adsr", 128, *this));
}

void setup_synth_bthree()
{
    SoundExternalFactory<SynthBThree> obj("synth.bee3~", OBJECT_FACTORY_DEFAULT);
}
