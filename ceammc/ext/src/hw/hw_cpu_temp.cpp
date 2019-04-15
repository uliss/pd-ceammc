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
#include "hw_cpu_temp.h"
#include "ceammc_factory.h"

HwCpuTemp::HwCpuTemp(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

#ifndef WITH_SMC
    OBJ_ERR << "supports only MacOSX at this moment";
#endif
}

void HwCpuTemp::onBang()
{
#ifdef WITH_SMC
    return floatTo(0, smc_.cpuTemperature());
#endif
}

void setup_hw_cpu_temp()
{
    ObjectFactory<HwCpuTemp> obj("hw.cpu_temp");
}
