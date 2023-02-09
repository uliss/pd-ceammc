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
    , from_(0)
    , to_(0)
{
    createOutlet();

    from_ = new FloatProperty("@from", 0);
    from_->setArgIndex(0);
    to_ = new FloatProperty("@to", 1);
    to_->setArgIndexNext(from_);

    addProperty(from_);
    addProperty(to_);
}

void CC2Amp::onFloat(t_float v)
{
    floatTo(0, convert(v));
}

t_float CC2Amp::convert(t_float v)
{
    if (v <= 0)
        return from_->value();

    if (v >= 127)
        return to_->value();

    return convert::lin2lin<t_float>(v, 0, 127, from_->value(), to_->value());
}

void setup_conv_cc2amp()
{
    ObjectFactory<CC2Amp> obj("conv.cc2amp");
    obj.addAlias("cc->amp");

    obj.setDescription("convert from MIDI Control Change range");
    obj.setCategory("conv");
    obj.setKeywords({"conv", "cc", "amp"});
}
