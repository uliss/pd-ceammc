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
#include "conv_sec2samp.h"
#include "ceammc_factory.h"

SecondToSample::SecondToSample(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void SecondToSample::onFloat(t_float v)
{
    floatTo(0, v * sys_getsr());
}

void SecondToSample::onList(const AtomList& lst)
{
    listTo(0, lst * sys_getsr());
}

void setup_conv_sec2samp()
{
    ObjectFactory<SecondToSample> obj("conv.sec2samp");
    obj.addAlias("sec->samp");
}
