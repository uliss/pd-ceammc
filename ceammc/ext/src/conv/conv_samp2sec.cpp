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
#include "conv_samp2sec.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

SampleToSec::SampleToSec(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void SampleToSec::onFloat(t_float v)
{
    floatTo(0, v / sys_getsr());
}

void SampleToSec::onList(const AtomListView& lv)
{
    SmallAtomList res;
    res.reserve(lv.size());
    for (auto& a : lv)
        res.push_back(a / sys_getsr());

    listTo(0, res.view());
}

void setup_conv_samp2sec()
{
    ObjectFactory<SampleToSec> obj("conv.samp2sec");
    obj.addAlias("samp->sec");

    obj.setDescription("convert samples to time according to samplerate");
    obj.setCategory("conv");
    obj.setKeywords({"conv", "samp"});
}
