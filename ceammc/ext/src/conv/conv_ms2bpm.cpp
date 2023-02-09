/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "conv_ms2bpm.h"
#include "ceammc_factory.h"

MsToBpm::MsToBpm(const PdArgs& a)
    : BpmToMs(a)
{
}

void setup_conv_ms2bpm()
{
    ObjectFactory<MsToBpm> obj("conv.ms2bpm");
    obj.addAlias("ms->bpm");

    obj.setXletsInfo({ "float: period in ms" }, { "float: bpm" });

    obj.setDescription("convert period in milliseconds to frequency in BPM");
    obj.setCategory("conv");
    obj.setKeywords({"conv", "time"});
}
