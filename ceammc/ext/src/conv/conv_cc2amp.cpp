/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "conv_cc2amp.h"
#include "ceammc_convert.h"

CC2Amp::CC2Amp(const PdArgs& a)
    : BaseObject(a)
    , v0_(0)
    , v1_(0)
    , clip_(0)
{
    createOutlet();

    v0_ = new FloatProperty("@v0", 0);
    v1_ = new FloatProperty("@v1", 1);
    clip_ = new BoolProperty("@clip", true);

    createProperty(v0_);
    createProperty(v1_);
    createProperty(clip_);

    parseArguments();
    if (args().size() == 2) {
        if (!a.args.hasProperty("@v0") && !a.args.hasProperty("@v1")) {
            v0_->setValue(args()[0].asFloat(0));
            v1_->setValue(args()[1].asFloat(1));
        }
    }
}

void CC2Amp::onFloat(t_float v)
{
    floatTo(0, convert(v));
}

t_float CC2Amp::convert(t_float v)
{
    if (clip_->value())
        v = clip<t_float>(v, 0, 127);
    return convert::lin2lin<t_float>(v, 0, 127, v0_->value(), v1_->value());
}

extern "C" void setup_conv0x2ecc2amp()
{
    ObjectFactory<CC2Amp> obj("conv.cc2amp");
    obj.addAlias("cc->amp");
}
