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
#include "conv_bpm2hz.h"
#include "ceammc_factory.h"

BpmToHz::BpmToHz(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void BpmToHz::onFloat(t_float v)
{
    floatTo(0, v / 60);
}

void BpmToHz::onList(const AtomList& lst)
{
    listTo(0, lst / 60);
}

void setup_conv_bpm2hz()
{
    ObjectFactory<BpmToHz> obj("conv.bpm2hz");
    obj.addAlias("bpm->hz");
}
