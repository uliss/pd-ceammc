/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "conv_semi2ratio.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

static inline t_float semi2ratio(t_float x)
{
    return std::exp2(x / 12);
}

ConvSemiToRatio::ConvSemiToRatio(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ConvSemiToRatio::onFloat(t_float f)
{
    floatTo(0, std::exp2(f / 12));
}

void ConvSemiToRatio::onList(const AtomListView& lv)
{
    SmallAtomList res;
    res.reserve(lv.size());

    lv.mapFloat(semi2ratio, res);
    listTo(0, res.view());
}

void setup_conv_semi2ratio()
{
    ObjectFactory<ConvSemiToRatio> obj("conv.semi2ratio");
    obj.addAlias("semi->ratio");
}
