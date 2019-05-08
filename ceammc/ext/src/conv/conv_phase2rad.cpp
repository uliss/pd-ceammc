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
#include "conv_phase2rad.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

PhaseToRad::PhaseToRad(const PdArgs& args)
    : SimpleConverter(args, convert::phase2rad<t_float>)
{
}

PhaseToRadTilde::PhaseToRadTilde(const PdArgs& args)
    : SimpleConverterTilde(args, convert::phase2rad<t_float>)
{
}

void setup_conv_phase2rad()
{
    ObjectFactory<PhaseToRad> obj("conv.phase2rad");
    obj.addAlias("phase->rad");

    SoundExternalFactory<PhaseToRadTilde> obj_t("conv.phase2rad~");
    obj_t.addAlias("phase->rad~");
}
