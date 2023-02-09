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
#include "conv_sec2bpm.h"
#include "ceammc_factory.h"

SecToBpm::SecToBpm(const PdArgs& a)
    : BpmToSec(a)
{
}

void setup_conv_sec2bpm()
{
    ObjectFactory<SecToBpm> obj("conv.sec2bpm");
    obj.addAlias("sec->bpm");

    obj.setXletsInfo({ "float: period in seconds" }, { "float: bpm" });

    obj.setDescription("convert period in seconds to frequency in BPM");
    obj.setCategory("conv");
    obj.setKeywords({"conv", "time"});
}
