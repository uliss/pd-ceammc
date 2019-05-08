/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "conv_angles.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

Phase2Rad::Phase2Rad(const PdArgs& args)
    : SimpleConverter(args, convert::phase2rad<t_float>)
{
}

Rad2Phase::Rad2Phase(const PdArgs& args)
    : SimpleConverter(args, convert::rad2phase<t_float>)
{
}

Phase2RadTilde::Phase2RadTilde(const PdArgs& args)
    : SimpleConverterTilde(args, convert::phase2rad<t_float>)
{
}

Rad2PhaseTilde::Rad2PhaseTilde(const PdArgs& args)
    : SimpleConverterTilde(args, convert::rad2phase<t_float>)
{
}

void setup_conv_angles()
{
    ObjectFactory<Phase2Rad> p2r("conv.phase2rad");
    p2r.addAlias("phase->rad");

    ObjectFactory<Rad2Phase> r2p("conv.rad2phase");
    r2p.addAlias("rad->phase");

    SoundExternalFactory<Phase2RadTilde> p2rt("conv.phase2rad~");
    p2rt.addAlias("phase->rad~");

    SoundExternalFactory<Rad2PhaseTilde> r2pt("conv.rad2phase~");
    r2pt.addAlias("rad->phase~");
}
