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
#include "conv_bpm2ms.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

static t_float bpm2ms(t_float x)
{
    return x == 0
        ? std::numeric_limits<t_float>::max()
        : (60000 / x);
}

BpmToMs::BpmToMs(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void BpmToMs::onFloat(t_float v)
{
    floatTo(0, bpm2ms(v));
}

void BpmToMs::onList(const AtomListView& lv)
{
    SmallAtomList res;

    lv.mapFloat([](t_float x) -> t_float { return bpm2ms(x); }, res);
    listTo(0, res.view());
}

void setup_conv_bpm2ms()
{
    ObjectFactory<BpmToMs> obj("conv.bpm2ms");
    obj.addAlias("bpm->ms");

    obj.setXletsInfo({ "float: bpm" }, { "float: period in ms" });

    obj.setDescription("convert frequency in BPM to period in milliseconds");
    obj.setCategory("conv");
    obj.setKeywords({ "conv", "time" });
}
