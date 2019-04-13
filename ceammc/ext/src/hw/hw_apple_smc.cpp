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
#include "hw_apple_smc.h"
#include "ceammc_factory.h"

HwAppleSMC::HwAppleSMC(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    createCbProperty("@keys", &HwAppleSMC::propKeys);

#ifndef WITH_SMC
    OBJ_ERR << "no system management control support for this device";
#endif
}

AtomList HwAppleSMC::propKeys() const
{
#ifdef WITH_SMC
    return smc_.keys();
#else
    return AtomList();
#endif
}

void setup_hw_apple_smc()
{
    ObjectFactory<HwAppleSMC> obj("hw.apple_smc");
}
